
(define x 1)
(define y 2)

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

(eval-file "unittests.lisp")

(define not (lambda (e) (if e #f #t)))
