(class Point
  private ;Everything below this point is inaccessible from the outside
  (class-properties
    (x 0)
    (y 0)
  )
  public ;Everything below this point is accessible from the outside. Leverages out the 'private' statement above.
  (class-properties
    (lambda(set-x newX) (set! x newX))
    (lambda(get-x) x)
    (lambda(set-y newY) (set! y newY))
    (lambda(get-y) y)
  )
  (class-events
    (lambda (ctor x y)
    	(set-x x)
    	(set-y y)
    )
    (lambda (call rhs); With this event, a Point instance can be used like this: (p0) => "{ 0, 0 }"
    	(format "{ %d, %d }" (get-x) (get-y))
    )
  )
)

(define p0 (Point 10 20))
