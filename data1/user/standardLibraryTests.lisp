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
                        (begin (set! result (* n result))
                               (fac-helper (- n 1) result))))
            (fac-helper n 1))

    (assert (= (fac 0) 1))
    (assert (= (fac 1) 1))
    (assert (= (fac 2) 2))
    (assert (= (fac 3) 6))
    (assert (= (fac 4) 24))
    (assert (= (fac 5) 120))
    (assert (= (fac 6) 720))

    ; This function is used to test whether a recursive call will alter the caller's environment, which should not happen
    ; For ANY input of \a n, n should be returned without modification.
    (define (f n abort)
            (if abort
                n
                (f (-- n) (not abort)))
            ; Always return the original input
            n)

    (assert (= (f 42 true) 42))
    (assert (= (f 42 false) 42))

    (print "=== Standard library unit tests completed successfully.")
))
