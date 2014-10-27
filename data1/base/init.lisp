
(define (file.eval name) (eval (read (file.read-string name))))
(file.eval "standardLibrary.lisp")
(file.eval "contTests.lisp")
(file.eval "prime.lisp")
(computePrimes 10)
(gc.collect)
(computePrimes 10)
(gc.collect)
