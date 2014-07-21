
(define (eval-file fileName)
    (define f (file-open fileName))
    (define content (file-read-string f))
    (define result (eval (read content)))
    (file-close f)
    result
)

(define (not e) (if e #f #t))
(define (xor a b) (not (or (not (or a b)) (and a b))))

(define first car)
(define rest cdr)

; Non-standard
(define == =)
(define % modulo)

(eval-file "unittests.lisp")
