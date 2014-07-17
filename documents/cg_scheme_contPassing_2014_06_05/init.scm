(display "hier init.scm\n")

;
;; define common useful functions
;;

(define (not a) (if a #f #t))
(define (fac n) (if (eq? n 1) 1 (* n (fac (- n 1)))))

(define (countDown n)
    (if (eq? n 0)
	"fertig"
	(countDown (- n 1))))
