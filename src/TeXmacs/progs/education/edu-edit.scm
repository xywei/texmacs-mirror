
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : edu-edit.scm
;; DESCRIPTION : editing routines for educational purposes
;; COPYRIGHT   : (C) 2019  Joris van der Hoeven
;;
;; This software falls under the GNU general public license version 3 or later.
;; It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
;; in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(texmacs-module (education edu-edit)
  (:use (dynamic fold-edit)
        (education edu-drd)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Context predicates
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (problem-context? t)
  (exercise-tag? (tree-label t)))

(tm-define (solution-context? t)
  (solution-tag? (tree-label t)))

(tm-define (mc-context? t)
  (mc-tag? (tree-label t)))

(tm-define (mc-exclusive-context? t)
  (mc-exclusive-tag? (tree-label t)))

(tm-define (mc-plural-context? t)
  (mc-plural-tag? (tree-label t)))

(tm-define (with-button-context? t)
  (with-button-tag? (tree-label t)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Operating on a tree
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (edu-operate-document l mode)
  (cond ((null? l) (noop))
        ((and (nnull? (cdr l))
              (problem-context? (car l))
              (solution-context? (cadr l)))
         (edu-operate-document (cddr l) mode)
         (let* ((prb (car l))
                (sol (cadr l))
                (tag (if (== mode :problem) 'folded 'unfolded)))
           (tree-insert-node! prb 0 (list tag))
           (tree-insert-node! sol 0 (list tag))
           (tree-join (tree-up prb) (tree-index prb))))
        (else
         (edu-operate-document (cdr l) mode)
         (edu-operate (car l) mode))))

(tm-define (edu-operate t mode)
  (cond ((tree-atomic? t) (noop))
        ((toggle-first-context? t)
         (cond ((== mode :solution)
                (dynamic-operate t :unfold))))
        ((toggle-second-context? t)
         (cond ((== mode :problem)
                (dynamic-operate t :fold))))
        ((tree-is? t 'document)
         (edu-operate-document (tree-children t) mode))
        (else
          (for-each (cut edu-operate <> mode) (tree-children t)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Setting the main operation mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (edu-set-mode mode)
  (edu-operate (buffer-tree) mode))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Basic editing of multiple choice lists
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(tm-define (make-mc env)
  (insert-go-to `(document (,env (mc-field "false" ""))) '(0 0 1 0)))

(tm-define (structured-horizontal? t)
  (:require (mc-context? t))
  #t)

(tm-define (focus-can-insert? t)
  (:require (mc-context? t))
  #t)

(tm-define (focus-can-remove? t)
  (:require (mc-context? t))
  #t)

(define (insert-mc-field t forwards?)
  (let* ((p (tree->path t))
	 (i (tree-down-index t))
	 (d (if forwards? 1 0)))
    (tree-insert! t (+ i d) '((mc-field "false" "")))
    (go-to (append p (list (+ i d) 1 0)))))

(tm-define (kbd-enter t shift?)
  (:require (mc-context? t))
  (if shift? (former t shift?) (insert-mc-field #t)))

(tm-define (structured-insert-horizontal t forwards?)
  (:require (mc-context? t))
  (insert-mc-field t forwards?))

(tm-define (structured-insert-vertical t downwards?)
  (:require (mc-context? t))
  (insert-mc-field t forwards?))

(define (remove-mc-field t forwards? structured?)
  (let* ((i (tree-down-index t))
	 (n (tree-arity t)))
    (cond ((> n 1)
	   (cond ((and structured? (not forwards?) (> i 0))
		  (set! i (- i 1)))
		 ((and forwards? (< i (- n 1)))
		  (tree-go-to t (+ i 1) :start))
		 ((and forwards? (== i (- n 1)))
		  (tree-go-to t (+ i -) :end))
		 ((and (not forwards?) (> i 0))
		  (tree-go-to t (- i 1) :end))
		 ((and (not forwards?) (== i 0))
		  (tree-go-to t (+ i 1) :start)))
	   (tree-remove t i 1))
	  ((with-button-context? (tree-up t))
	   (tree-cut (tree-up t)))
	  (else (tree-cut t)))))

(tm-define (kbd-backspace)
  (:require (and (== (cursor-tree) (tree ""))
		 (tree-is? (tree-up (cursor-tree)) 'mc-field)
		 (mc-context? (tree-up (tree-up (cursor-tree))))))
  (remove-mc-field (tree-up (tree-up (cursor-tree))) #f #f))

(tm-define (kbd-delete)
  (:require (and (== (cursor-tree) (tree ""))
		 (tree-is? (tree-up (cursor-tree)) 'mc-field)
		 (mc-context? (tree-up (tree-up (cursor-tree))))))
  (remove-mc-field (tree-up (tree-up (cursor-tree))) #t #f))

(tm-define (structured-remove-horizontal t forwards?)
  (:require (mc-context? t))
  (remove-mc-field t forwards? #t))

(tm-define (structured-remove-vertical t downwards?)
  (:require (mc-context? t))
  (remove-mc-field t forwards? #t))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Toggling buttons
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (clear-buttons t)
  (cond ((tree-func? t 'mc-field 2)
	 (tree-set (tree-ref t 0) (tree "false")))
	((tree-atomic? t) (noop))
	(else (for-each clear-buttons (tree-children t)))))

(define (handle-exclusive p i)
  (with t (path->tree p)
    (cond ((tree-atomic? t) (handle-exclusive (cDr p) (cAr p)))
	  ((tree-is? t 'mc-field) (handle-exclusive (cDr p) (cAr p)))
	  ((mc-exclusive-context? t)
	   (let* ((l (tree-children t))
		  (n (tree-arity t))
		  (x (append (sublist l 0 i) (sublist l (+ i 1) n))))
	     (for-each clear-buttons x))))))

(tm-define (mouse-toggle-button t)
  (:type (-> void))
  (:synopsis "Toggle a button using the mouse")
  (:secure #t)
  (if (tree->path t) (handle-exclusive (tree->path t) #f))
  (cond ((tm-equal? t "true" ) (tree-set! t "false"))	
	((tm-equal? t "false") (tree-set! t "true" ))))