
(define (file.eval name) (eval (read (file.read-string name))))
(file.eval "standardLibrary.lisp")
