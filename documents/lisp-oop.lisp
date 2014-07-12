(class Point
  private
  (class-properties
    (x 0)
    (y 0)
  )
  public
  (class-properties
    (lambda(set-x newX) (set! x newX))
    (lambda(get-x) x)
    (lambda(set-y newY) (set! y newY))
    (lambda(get-y) y)
  )
  (class-events
    ((ctor x y)
    	(set-x x)
    	(set-y y))
    ((add rhs)
    	(set-x (+ (get-x) rhs))
    	(set-y (+ (get-y) rhs)))
  )
)

(define p0 (Point 10 20))
