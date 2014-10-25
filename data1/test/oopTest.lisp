(define (newRect width height)
  (lambda (func)
    (if (eq? func 'set-width)
        (lambda (newWidth)
          (set! width newWidth)
        )
    (if (eq? func 'set-height)
        (lambda (newHeight)
          (set! height newHeight)
        )
    (if (eq? func 'get-width)
        (lambda () width)
    (if (eq? func 'get-height)
        (lambda () height)
    (if (eq? func 'get-area)
        (lambda () (* width height))
    'does-not-know)))))))
	
(define (oopTest) 
	(define rect1 (newRect 2 4))
	(define rect2 (newRect 3 5))

	(assert (= ((rect1 'get-width)) 2))
	(assert (= ((rect1 'get-height)) 4))
	(assert (= ((rect2 'get-width)) 3))
	(assert (= ((rect2 'get-height)) 5))
	(assert (= ((rect1 'get-area)) 8))
	(assert (= ((rect2 'get-area)) 15))
	((rect1 'set-width) 1)
	(assert (= ((rect1 'get-width)) 1))
	(assert (= ((rect1 'get-area)) 4))
	(assert (= ((rect2 'get-width)) 3))
	(assert (= ((rect2 'get-height)) 5))
	(assert (= ((rect2 'get-area)) 15))
	((rect2 'set-height) 6)
	(assert (= ((rect2 'get-height)) 6))
	(assert (= ((rect2 'get-area)) 18))
)
(oopTest)