((lambda ()
    (print "=== Running unit tests for the standard library.")

    (define (fac n)
            (if (<= n 1)
                1
                (* (fac (- n 1)) n)))

    (assert (= (fac 0) 1))
    (assert (= (fac 1) 1))
    (assert (= (fac 2) 2))
    (assert (= (fac 3) 6))
    (assert (= (fac 4) 24))
    (assert (= (fac 5) 120))
    (assert (= (fac 6) 720))

    (define (fac n)
            (define (fac-helper n result)
                    (if (<= n 1)
                        result
                        (begin (set! result (* n result)) (fac-helper (- n 1)))))
            (fac-helper n 1))

    (assert (= (fac 0) 1))
    (assert (= (fac 1) 1))
    (assert (= (fac 2) 2))
    (assert (= (fac 3) 6))
    (assert (= (fac 4) 24))
    (assert (= (fac 5) 120))
    (assert (= (fac 6) 720))

    (print "=== Standard library unit tests completed successfully.")
))
