
(define eval-file
    (lambda (fileName)
        (eval
            (read
                ((lambda ()
                    (define f (file-open fileName))
                    (define content (file-read-string f))
                    (file-close f)
                    content
                ))
            )
        )
    )
)

(define (not e) (if e #f #t))
(define (xor a b) (not (or (not (or a b)) (and a b))))

(define first car)
(define rest cdr)

; Non-standard
(define == =)

(eval-file "unittests.lisp")
