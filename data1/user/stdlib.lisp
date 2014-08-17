
(define nil null)

(define (not e) (if e #f #t))
(define (xor a b) (not (or (not (or a b)) (and a b))))

(define first car)
(define rest cdr)

(define true #t)
(define false #f)

(define == =)
(define (!= lhs rhs) (not (= lhs rhs)))
