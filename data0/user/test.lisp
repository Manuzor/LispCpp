(set! counter 0)
(define (f n)
        (print counter)
        (set! counter (+ counter 1))
        (if (<= n 0)
            n
            (f (f (- n 1)))))

(print "==> Begin Call ===")
(f 3)
(print "==> End Call =====")
