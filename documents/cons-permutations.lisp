(define nil '())

(define a0 (cons nil nil))
(define a1 (cons 1 nil))
(define a2 (cons nil 2))
(define a3 (cons 1 2))

(define b0 (cons nil (cons nil nil)))
(define b1 (cons 1   (cons nil nil)))
(define b2 (cons 1   (cons 2   nil)))
(define b3 (cons 1   (cons nil 3  )))
(define b4 (cons 1   (cons 2   3  )))
(define b5 (cons nil (cons 2 nil  )))
(define b6 (cons nil (cons nil 3  )))
(define b7 (cons nil (cons 2   3  )))
(define b8 (cons 1   (cons 2   3  )))

(define c0 (cons (cons nil nil) nil))
(define c1 (cons (cons nil nil) 1  ))
(define c2 (cons (cons 2   nil) 1  ))
(define c3 (cons (cons nil 3  ) 1  ))
(define c4 (cons (cons 2   3  ) 1  ))
(define c5 (cons (cons 2 nil  ) nil))
(define c6 (cons (cons nil 3  ) nil))
(define c7 (cons (cons 2   3  ) nil))
(define c8 (cons (cons 2   3  ) 1  ))

a0 a1 a2 a3
b0 b1 b2 b3 b4 b5 b6 b7 b8
c0 c1 c2 c3 c4 c5 c6 c7 c8
