(define (computePrimes num)
    (define (isPrime primes cur)
		(if (eq? primes nil)
			;then
			true
			;else
			(if (== (% cur (first primes)) 0)
				;then
				false
				;else
				(isPrime (rest primes) cur)
			)
		)
    )
    (define (helper primes cur i)
        (if (== num i)
            ;then
            primes
            ;else
            (if (isPrime primes cur)
                ;then
                (helper
					(cons cur primes)
					(+ cur 1)
					(+ i 1))
                ;else
                (helper primes (+ cur 1) i)
            )
        )
    )
    (helper (cons 2 nil) 3 1)
)