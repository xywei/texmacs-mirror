
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : server-chat.scm
;; DESCRIPTION : Sending messages and chatting, server side
;; COPYRIGHT   : (C) 2020  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (server server-chat)
  (:use (server server-tmfs)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Chat rooms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (chat-room-create uid name)
  (with-time-stamp #t
    (db-create-entry `(("type" "chat-room")
                       ("name" ,name)
                       ("owner" ,uid)
                       ("readable" "all")
                       ("writable" "all")))))

(tm-service (remote-chat-room-create name)
  ;; Create chat room and return its identifier
  ;;(display* "remote-chat-room-create " name "\n")
  (with uid (server-get-user envelope)
    (with crid (chat-room-create uid name)
      (server-return envelope crid))))

(define (chat-room-id name)
  (with l (db-search `(("type" "chat-room")
                       ("name" ,name)))
    (and (nnull? l) (car l))))

(define chat-room-messages (make-ahash-table))
(define chat-room-present  (make-ahash-table))

(define (chat-message-retrieve mid)
  (let* ((action (db-get-field-first mid "action" "unknown"))
         (msg (db-get-field-first mid "message" "unknown"))
         (from (db-get-field-first mid "from" "unknown"))
         (pseudo (db-get-field-first from "pseudo" "unknown"))
         (full (db-get-field-first from "name" "unknown"))
         (date (db-get-field-first mid "date" "unknown")))
    (when (== action "send")
      (with doc (string-load (repository-get msg))
        (set! msg (convert doc "texmacs-snippet" "texmacs-stree"))))
    (list action pseudo full date msg)))

(define (chat-room-retrieve crid)
  (with l (db-search `(("type" "chat-message")
                       ("to" ,crid)
                       (:order "date" #t)))
    (map chat-message-retrieve l)))

(define (chat-room-initialize crid)
  (when (not (ahash-ref chat-room-messages crid))
    (ahash-set! chat-room-messages crid (chat-room-retrieve crid))
    (ahash-set! chat-room-present  crid (list))))

(tm-service (remote-chat-room-open name)
  ;; Connect client to a chat room and return list of past messages
  ;;(display* "remote-chat-room-open " name "\n")
  (with (client msg-id) envelope
    (with crid (chat-room-id name)
      (if (not crid)
          (server-error envelope "Error: unknown chat room")
          (begin
            (chat-room-initialize crid)
            (with l (ahash-ref chat-room-present crid)
              (when (nin? client l)
                (ahash-set! chat-room-present crid (cons client l))))
            (server-return envelope (ahash-ref chat-room-messages crid)))))))

(define (chat-room-notify mid)
  ;; Notify the arrival of a new message to all participants
  (let* ((crid (db-get-field-first mid "to" "unknown"))
         (name (db-get-field-first crid "name" "unknown"))
         (dummy (chat-room-initialize crid))
         (old-l (ahash-ref chat-room-messages crid))
         (new-m (chat-message-retrieve mid))
         (new-l (rcons old-l new-m)))
    (ahash-set! chat-room-messages crid new-l)
    (for (client (ahash-ref chat-room-present crid))
      (server-remote-eval client `(chat-room-receive ,name ,new-m)
        (lambda (ok?) (noop))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sending messages
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (remote-create-message uid msg)
  (let* ((rid (with-time-stamp #t
                (db-create-entry `(("type" "message")
                                   ("owner" ,uid)))))
         (name (repository-add rid "tm"))
         (fname (repository-get rid))
         (doc (convert msg "texmacs-stree" "texmacs-snippet")))
    (string-save doc fname)
    rid))

(define (remote-send uid dest action msg)
  (cond ((== action "send-document")
         (and-with msg* (remote-create-message uid msg)
           (remote-send uid dest "send" msg*)))
        ((list? dest)
         (for (did dest)
           (remote-send uid did action msg)))
        ((== (db-get-field-first dest "type" #f) "chat-room")
         (with-time-stamp #t
           (with mid (db-create-entry `(("type" "chat-message")
                                        ("action" ,action)
                                        ("from" ,uid)
                                        ("to" ,dest)
                                        ("message" ,msg)))
             (chat-room-notify mid))))
        ((chat-room-id dest)
         (remote-send uid (chat-room-id dest) action msg))))

(tm-service (remote-send-message dest action msg)
  ;;(display* "remote-send-message " dest ", " action ", " msg "\n")
  (with uid (server-get-user envelope)
    (remote-send uid dest action msg)
    #t))
