
(define (a) (b))
(define (b) (print-cont-chain))
(define (c) (print-cont-chain))
(print "c: ")
(c)
(print "a: ")
(a)

(define (d) (if #t (b) (c)))
(define (e) (if #f (b) (c)))

(print "d: ")
(d)
(print "e: ")
(e)

(define (isPrime primes cur)
	(gc.collect)
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
