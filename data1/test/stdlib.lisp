; negates a given boolean value
(define (not boolean)
  (if boolean #f #t)
)

; generates containing all numbers from <start> to <end> with <step> space in between the numbers
(define (iota start end step)
	(define (helper list i)
		(if (= i start)
			(cons i list)
			(helper (cons i list) (- i step))
		)
	)
	(if (= start end)
		null
		(helper null (- end step))
	)
)

; reverses a list
(define (reverse list)
	(define (helper result remaining)
		(if (pair? remaining)
			(helper (cons (car remaining) result) (cdr remaining))
			result
		)
	)
	(helper null list)
)

; maps elements to new values
(define (map list func)
	(define (helper result remaining)
		(if (pair? remaining)
			(helper (cons (func (car remaining)) result) (cdr remaining))
			result
		)
	)
	(reverse (helper null list))
)

; filters elements of a list
(define (filter list condition)
	(define (helper result remaining)
		(if (pair? remaining)
			(if (condition (car remaining))
				(helper (cons (car remaining) result) (cdr remaining))
				(helper result (cdr remaining))
			)
			result
		)
	)
	(reverse (helper null list))
)

; computes the length of a list
(define (length list)
	(define (helper sum remaining)
		(if (pair? remaining)
			(helper (+ sum 1) (cdr remaining))
			sum
		)
	)
	(helper 0 list)
)

; appends one list to another
(define (append list1 list2)
  (define (helper appendTo appendFrom)
    (if (pair? appendFrom)
        (helper (cons (car appendFrom) appendTo) (cdr appendFrom))
        appendTo
    )
  )
  (helper list2 (reverse list1))
)

; pratitions the list into elements that satisfy the condition and elements that don't
(define (partition list condition)
  (define (helper matching nonMatching remaining)
    (if (pair? remaining)
	  (if (condition (car remaining))
	    (helper (cons (car remaining) matching) nonMatching (cdr remaining))
		(helper matching (cons (car remaining) nonMatching) (cdr remaining))
      )
	  (append (reverse matching) (reverse nonMatching))
	)
  )
  (helper null null list)
)

; returns the tail of the list omitting k elements at the start
(define (list-tail list k)
  (if (and (pair? list) (> k 0))
    (list-tail (cdr list) (- k 1))
    list
  )
)

; reduces a given list using the given action
(define (reduce list action)
  (define (helper cur remaining)
    (if (pair? remaining)
      (helper (action (car remaining) cur) (cdr remaining))
      cur
    )
  )
  (helper (car list) (cdr list))
)

; measures the execution time of the given lambda and prints it to stdout
; use like this: (measureTime (lambda () codeToMeasure))
(define (measureTime l)
  (define timer (startTimer))
  (l)
  (format "exeuction time {0} ms" (stopTimer timer))
)

; returns the last element in a list
(define (last l)
  (define (helper cur remaining)
    (if (pair? remaining)
		(helper (car remaining) (cdr remaining))
		cur
	)
  )
  (helper null l)
)


; test routine for the stdlib
(define (vmTest actual expected) (assert (eqv? actual expected)))
(define (stdlib-test)
  (vmTest (last '(1 2 3)) 3)
  (vmTest (eq? (last null) null) #t)
  (vmTest (partition '(1 2 3 4) (lambda (x) (= (% x 2) 0))) '(2 4 1 3))
  (vmTest (iota 0 4 1) '(0 1 2 3))
  (vmTest (iota 5 0 -1) '(5 4 3 2 1))
  (vmTest (reverse '(1 2 3)) '(3 2 1))
  (vmTest (map '(1 2 3) (lambda (x) (+ x 1))) '(2 3 4))
  (vmTest (filter (iota 0 10 1) (lambda (x) (= (% x 2) 0))) '(0 2 4 6 8))
  (vmTest (append '(1 2) '(3 4 5)) '(1 2 3 4 5))
  (vmTest (length '(1 2 3)) 3)
  (vmTest (length null) 0)
  (vmTest (reduce '(1 2 3) +) 6)
  (vmTest (list-tail '(1 2 3 4 5) 2) '(3 4 5))
)
(stdlib-test)
