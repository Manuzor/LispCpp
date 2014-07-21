(set-recursion-limit 100)
(set! counter 0)
(define (tak x y z)
    (set! counter (+ counter 1))
    (print (format "call #{0}: {1} {2} {3}" counter x y z))
    (if (>= y x) ; y >= x
        z
        (tak (tak (- x 1) y z)
             (tak (- y 1) z x)
             (tak (- z 1) x y))))
(tak 4 1 0)
tak
