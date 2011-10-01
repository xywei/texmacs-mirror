
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : graphics-single.scm
;; DESCRIPTION : editing routines for single graphical objects
;; COPYRIGHT   : (C) 2001  Joris van der Hoeven
;;               (C) 2004, 2005, 2006  Joris van der Hoeven and Henri Lesourd
;;               (C) 2011  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (graphics graphics-single)
  (:use (graphics graphics-object)
        (graphics graphics-env)
        (graphics graphics-main)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Basic operations
;;
;; NOTE: Imperative functions, which inconditionnaly perform a given
;;   operation on the sketch.
;;
;;   These functions depend on, and can change the current edit state (i.e.,
;;   being in modifying or in selecting mode), and they maintain the value
;;   of the state variables current-point-no and current-edge-sel?, which
;;   are basic pointers inside the edited object.
;;
;;   In other words, these functions are methods which operate on the sketch,
;;   according to what you need to do when editing in point mode.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Basic operations (setting the object)
(define (object-set! o . opt)
  (set! layer-of-last-removed-object #f)
  (set! current-obj o)    ;; FIXME: Hmmm, I still have a doubt for this one.
                          ;;   Should completely clarify its role & centralize
                          ;;   where it's managed (and for similar gvs as well).
  (if sticky-point
      (sketch-set! `(,o))
      (if (in? 'checkout opt)
	  (begin
	     (sketch-set! `(,o))
	     (sketch-checkout))
	  (if (in? 'new opt)
	      (graphics-group-enrich-insert o)
	      (graphics-assign current-path o)))))

;; Basic operations (create)
(define (sketch-get1)
  (if (not (and (pair? (sketch-get)) (eq? 1 (length (sketch-get)))))
      (graphics-error "(sketch-get1)"))
  (sketch-get))
  
(tm-define (object_create tag x y)
  (:require (eq? tag 'point))
  (object-set! `(point ,x ,y) 'new))

(tm-define (object_create tag x y)
  (:require (in? tag gr-tags-curves))
  (with o (graphics-enrich `(,tag (point ,x ,y) (point ,x ,y)))
    (graphics-store-state 'start-create)
    (set! current-point-no 1)
    (object-set! o 'checkout)
    (graphics-store-state #f)))

(tm-define (object_create tag x y)
  (:require (eq? tag 'text-at))
  (object-set! `(text-at "" (point ,x ,y)) 'new))

(define (set-point-sub obj no x y)
  ;;(display* "set-point-sub " obj ", " no ", " x ", " y "\n")
  (cond ((== (car obj) 'with)
         (set-point-sub (cAr obj) no x y))
        ((== (car obj) 'point)
	 (set-car! (cdr obj) x)
	 (set-car! (cddr obj) y))
        ((and (not (not no)) (list? obj) (> (length obj) (+ no 1)))
         (set-point-sub (list-ref obj (+ no 1)) #f x y))
        (else #f)))

;; Basic operations (set & add point)
(define (object_set-point no xcur ycur)
  (define obj (stree-radical (car (sketch-get1))))
  (set-point-sub obj no xcur ycur)
  (object-set! (car (sketch-get))))

(define (object_add-point no xcur ycur x y dirn)
  (define obj (stree-radical (car (sketch-get1))))
  (if (not (and (in? (car obj) '(arc carc)) (> (length obj) 3)))
      (with l (list-tail (cdr obj) no)
  	(graphics-store-state #f)
 	(if dirn
 	    (begin
 	      (set-cdr! l (cons `(point ,x ,y) (cdr l)))
 	      (if (and xcur ycur)
 		  (set-car! l `(point ,xcur ,ycur)))
 	      (set! current-point-no (+ no 1)))
  	    (begin
  	      (set-cdr! l (cons (car l) (cdr l)))
  	      (set-car! l `(point ,x ,y))
 	      (if (and xcur ycur)
 		  (set-car! (cdr l) `(point ,xcur ,ycur)))
 	      (set! current-point-no no))
        )
	(object-set! (car (sketch-get)))
  	(set! current-edge-sel? #t))))

;; Basic operations (remove)
(define (object_remove-point no)
;; FIXME: should read the radical & memoize it
  (with l (if (<= no 0)
	      current-obj
	      (list-tail (cdr current-obj) (- no 1)))
    (set-cdr! l (cddr l))
    (set! current-point-no #f)
    (object-set! current-obj)))
  ;; FIXME: Should assign the memoized radical, here

(define (object_remove)
  (graphics-remove current-path))

;; Basic operations (checkout & commit)
(define (object_checkout)
  (sketch-set! `(,(path->tree current-path)))
  (sketch-checkout)
  ;;(display* "Checked out " (sketch-get) "\n")
  (sketch-set! (map tree->stree (sketch-get))))

(define (object_commit)
  (define obj (stree-radical (car (sketch-get1))))
  (if (not (and (in? (car obj) '(arc carc)) (<= (length obj) 3)))
      (begin
        (graphical-fetch-props (car (sketch-get)))
        (set! obj (graphics-enrich-bis
                   obj
                   graphical-id
                   graphical-opacity
                   graphical-color
                   graphical-pstyle
                   graphical-lwidth
                   (local-magnification graphical-magnification)
                   graphical-lstyle
                   graphical-lstyle-unit
                   graphical-larrows
                   graphical-fcolor
                   graphical-textat-halign
                   graphical-textat-valign))
        (set! current-edge-sel? #f)
        (sketch-set! `(,obj))
        ;;(display* "Commited " (sketch-get) "\n")
        (sketch-commit)
        (set! leftclick-waiting #f)
        (set! current-obj (stree-radical obj))
        (set! current-point-no #f)
        (graphics-forget-states)))
  (delayed
    (graphics-update-constraints)))

(define (current-in? l)
  (and (pair? current-obj) (in? (car current-obj) l)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Updating the constraints
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (tree-update-constraints t)
  (cond ((not (tree? t)) (noop))
        ((tree-atomic? t) (noop))
        ((and (match? t '(with "gid" :%1 (point :%2)))
              (graphics-has? (tree-ref t 1)))
         (let* ((old (tree-ref t :last))
                (new (graphics-ref (tree-ref t 1))))
           (when (!= new old) (tree-set t :last new))
           (graphics-notify-update (tree-ref t 1))))         
        (else (for-each tree-update-constraints (tree-children t)))))

(tm-define (graphics-update-constraints)
  (when (graphics-needs-update?)
    (with-innermost t 'graphics
      (remove-undo-mark)
      (tree-update-constraints t)
      (add-undo-mark))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Edit operations
;;
;; NOTE: Intelligent functions, which take into account the state,
;;   the previous mouse clicks, etc. They also perform printing the
;;   help messages. These functions maintain the value of the state
;;   variables, and they manage state stacking.
;;
;;   In other words, these functions implement the different states
;;   of the editing automaton.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define moveclick-tolerance "5px")
(define previous-leftclick #f)

(define (move-over)
  (set-message "Left click: add or edit object; Middle click: remove object"
               "")
  (graphics-decorations-update)
  (if current-path
      (with p2 (tm-upwards-path current-path '(text-at) '(graphics))
         (if (not p2) (go-to (rcons current-path 0))))))

(define (edit-insert x y)
  (object_create (cadr (graphics-mode)) x y))

(define (start-move)
  (define edge current-edge-sel?)
  (set-message "Left click: add point; Middle click: undo" "")
  (graphics-store-state 'start-move)
  (object_checkout)
  (graphics-group-start)
  (set! current-edge-sel? #t)
  (set! leftclick-waiting #f)
  (if (and edge
	   (not (and (current-in? '(arc carc))
                     (> (length current-obj) 3))))
      (begin
	 (object_add-point current-point-no #f #f current-x current-y #t)
	 (graphics-decorations-update)))
  (graphics-store-state #f))

(define (move-point)
  (if (and leftclick-waiting
	   (not (points-dist<
                 previous-leftclick
		 `(point ,current-x ,current-y)
                 moveclick-tolerance)))
      (begin
        (set! leftclick-waiting #f)
        (object_add-point
         current-point-no
         (cadr previous-leftclick) (caddr previous-leftclick)
         current-x current-y
         (== (logand (get-keyboard-modifiers) ShiftMask) 0)))
      (begin
        (if leftclick-waiting
            (set-message "Left click: finish" "")
            (set-message "Left click: add point; Middle click: undo" ""))
        (object_set-point current-point-no current-x current-y)))
  (graphics-decorations-update))

(define (last-point)
  (object_set-point
   current-point-no
   current-x
   current-y)
  (object_commit))

(define (next-point)
  (cond ((and leftclick-waiting
              (points-dist<
               previous-leftclick
               `(point ,current-x ,current-y)
               moveclick-tolerance))
         (last-point))
        ((and (not leftclick-waiting)
              previous-leftclick
              (points-dist<
               previous-leftclick
               `(point ,current-x ,current-y)
               moveclick-tolerance))
         (undo 0)
         (set! leftclick-waiting #f))
        (else
          (set-message "Left click: finish" "")
          (set! leftclick-waiting #t))))

(define (back)
  (graphics-back-state #f)
  (graphics-move current-x current-y))

(define (remove-point)
  (if (or (current-in? gr-tags-oneshot) (null? (cdddr current-obj))
	  (not (current-in? gr-tags-all))
	  (!= (logand (get-keyboard-modifiers) ShiftMask) 0))
      (begin
        (object_remove)
        (graphics-decorations-reset)
        (graphics-group-start))
      (begin
        (object_remove-point current-point-no)
        (graphics-decorations-update))))

;; Left button
(tm-define (click-non-sticky)
  (start-move))

(tm-define (click-non-sticky)
  (:require (current-in? '(text-at)))
  (if (== (graphics-mode) '(edit text-at))
      (begin
        (set-texmacs-pointer 'text-arrow)
        (go-to (car (select-first (s2f current-x) (s2f current-y)))))
      (begin
        (set-texmacs-pointer 'graphics-cross)
        (set! current-point-no 1)
        (start-move))))

(tm-define (click-sticky)
  (next-point))

(tm-define (click-sticky)
  (:require (current-in? '(text-at)))
  (object_commit))

(tm-define (left-button)
  (:require (or (current-in? gr-tags-point-curves)
                (current-in? '(text-at))
                (not (current-in? gr-tags-all))))
  (if sticky-point
      (click-sticky)
      (click-non-sticky)))

;; Move
(tm-define (move)
  (:require (current-in? gr-tags-point-curves))
  (if sticky-point
      (move-point)
      (move-over)))

(tm-define (move)
  (:require (current-in? '(text-at)))
  (set! current-point-no 1)
  (if sticky-point
      (move-point)
      (move-over)))

(tm-define (move)
  (:require (current-in? '(gr-group)))
  (if sticky-point
      (display* "Sticky move(gr-group) !yet implemented\n")
      (begin
	(set! current-point-no #f)
	(graphics-decorations-update))))

(tm-define (move)
  (:require (not (current-in? gr-tags-all)))
  (if sticky-point
      (move-point)
      (move-over)))

;; Middle button
(tm-define (middle-button)
  (if sticky-point
      (back)
      (remove-point)))

;; Dispatch
(tm-define (edit_left-button mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross)
  (if sticky-point
      (click-sticky)
      (edit-insert x y))
  (set! previous-leftclick `(point ,current-x ,current-y)))

(tm-define (edit_start-drag mode x y)
  (edit_left-button mode x y))

(tm-define (edit_start-drag mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross)
  (if (or sticky-point (current-in? '(text-at)) current-obj)
      (left-button))
  (set! previous-leftclick `(point ,current-x ,current-y)))

(tm-define (edit_end-drag mode x y)
  (edit_left-button mode x y))

(tm-define (edit_end-drag mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross)
  (if (or sticky-point (current-in? '(text-at)) current-obj)
      (last-point))
  (set! previous-leftclick `(point ,current-x ,current-y)))

(tm-define (edit_move mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross #t)
  (if current-obj
      (move)
      (graphics-decorations-reset)))

(tm-define (edit_drag mode x y)
  (edit_move mode x y))

(tm-define (edit_drag mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross #t)
  (if current-obj
      (move)
      (graphics-decorations-reset)))

(tm-define (edit_middle-button mode x y)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (set-texmacs-pointer 'graphics-cross)
  (when current-obj
    (middle-button)))

(tm-define (edit_right-button mode x y)
  (:require (eq? mode 'edit))
  (display* "Right button(edit) currently unused\n"))

(tm-define (graphics-update-tab)
  (:state graphics-state)
  (if current-obj (graphics-decorations-update)))

(tm-define (edit_tab-key mode inc)
  (noop))

(tm-define (edit_tab-key mode inc)
  (:require (eq? mode 'edit))
  (:state graphics-state)
  (if (and current-x current-y)
      (begin
        (select-next inc)
        (graphics-update-tab))
      (invalidate-graphical-object)))
