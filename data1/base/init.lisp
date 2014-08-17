
(define (eval-string-in env str)
        (eval-in env (read str)))

(define (eval-string str)
        (eval (read str)))

(define eval-file-handle null)
(define (eval-file fileName)
    (if (not (eq? eval-file-handle null))
        (file-close eval-file-handle))
    (set! eval-file-handle (file-open fileName))
    (if (eq? eval-file-handle null)
        null
        (begin
            (define content (file-read-string eval-file-handle))
            (define result (eval-string-in (env-get-global) content))
            (file-close eval-file-handle)
            (set! eval-file-handle null)
            result
        )
    )
)

(define (not e) (if e #f #t))
(define (xor a b) (not (or (not (or a b)) (and a b))))

(define first car)
(define rest cdr)

; Non-standard
(define == =)
(define % modulo)

;(eval-file "base/unittests.lisp")
