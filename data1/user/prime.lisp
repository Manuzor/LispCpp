(define (computePrimes num)
    (define (isPrime primes cur)
		(if (eq? primes null)
			;then
			#t
			;else
			(if (= (% cur (car primes)) 0)
				;then
				#f
				;else
				(isPrime (cdr primes) cur)
			)
		)
    )
    (define (helper primes cur i)
        ;(gc.collect)
        (if (= num i)
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
    (helper (cons 2 null) 3 1)
)
