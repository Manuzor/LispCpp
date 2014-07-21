
(print "Beginning unit tests.")

(assert (= 1 1))
(assert (not (= 2 1)))
(assert (> 2 1))
(assert (not (> 1 2)))
(assert (< 1 2))
(assert (not (< 2 1)))
(assert (<= 1 1))
(assert (<= 1 2))
(assert (>= 1 1))
(assert (>= 2 1))

(print "Finished unit tests.")
