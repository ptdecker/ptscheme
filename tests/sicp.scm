(define assert
    (lambda (x y)
        (eq? x y)))

(display "Testing: (+ 2 2) ... ")
(if (assert (+ 2 2) 4) (display "pass\n") (begin (display "fail\n") (exit 1)))
(exit)
