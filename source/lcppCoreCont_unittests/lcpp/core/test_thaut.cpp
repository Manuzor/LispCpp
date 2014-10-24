//////////////////////////////////////////////////////////////////////////
/// Most parts of this file's test cases are taken from                ///
/// the thLisp project by Benjamin Thaut.                              ///
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lcpp/foundation/exceptionBase.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp {
    namespace test
    {
        static void testExecute(const char* szToTest, const char* szExpected)
        {
            StackPtr<LispObject> pResult = evalString(szToTest, -1);
            StackPtr<LispObject> pString = object::toString(pResult);
            CUT_ASSERT.isTrue(ezStringUtils::IsEqual(szExpected, str::getValue(pString).GetData()));
        }
    }
}

LCPP_TestGroup(ThautTests);

LCPP_TestCase(ThautTests, Test_01)
{
    testExecute("(+ 1.25 0.5)", "1.75");
}

LCPP_TestCase(ThautTests, Test_02)
{
    testExecute("(+ 1 2)", "3");
}

LCPP_TestCase(ThautTests, Test_03)
{
    testExecute("(+ (+ 1 2) 3)", "6");
}

LCPP_TestCase(ThautTests, Test_04)
{
    testExecute("(lambda (x) x)", "<procedure>");
}

LCPP_TestCase(ThautTests, Test_05)
{
    testExecute("((lambda () 5))", "5");
}

LCPP_TestCase(ThautTests, Test_06)
{
    testExecute("((lambda (x) (+ x 1)) 1)", "2");
}

LCPP_TestCase(ThautTests, Test_07)
{
    testExecute("(define var 5) var", "5");
}

LCPP_TestCase(ThautTests, Test_08)
{
    testExecute("(define testNull ())", "#v");
}

LCPP_TestCase(ThautTests, Test_09)
{
    testExecute("((lambda () (define x 5) x))", "5");
}

LCPP_TestCase(ThautTests, Test_10)
{
    testExecute("((lambda () (define x 5) (+ x 1)))", "6");
}

LCPP_TestCase(ThautTests, Test_11)
{
    testExecute("((lambda () (define x (+ 2 3)) x))", "5");
}

LCPP_TestCase(ThautTests, Test_12)
{
    testExecute("((lambda () (define x (+ 2 3)) (+ x 1)))", "6");
}

LCPP_TestCase(ThautTests, Test_13)
{
    testExecute("(define add (lambda (x y) (+ x y))) (add 4 7)", "11");
}

LCPP_TestCase(ThautTests, Test_14)
{
    testExecute("(define (add1 x) (+ x 1)) (add1 3)", "4");
}

LCPP_TestCase(ThautTests, Test_15)
{
    testExecute("(if #t 1 2)", "1");
}

LCPP_TestCase(ThautTests, Test_16)
{
    testExecute("(if #f 1 2)", "2");
}

LCPP_TestCase(ThautTests, Test_17)
{
    testExecute("(if #t (if #t 1 2) 3)", "1");
}

LCPP_TestCase(ThautTests, Test_18)
{
    testExecute("(if #t (if #f 1 2) 3)", "2");
}

LCPP_TestCase(ThautTests, Test_19)
{
    testExecute("(if #f (if #f 1 2) 3)", "3");
}

LCPP_TestCase(ThautTests, Test_20)
{
    testExecute("(if #t (if #t 1 2) (if #t 3 4))", "1");
}

LCPP_TestCase(ThautTests, Test_21)
{
    testExecute("(if #t (if #f 1 2) (if #t 3 4))", "2");
}

LCPP_TestCase(ThautTests, Test_22)
{
    testExecute("(if #f (if #t 1 2) (if #t 3 4))", "3");
}

LCPP_TestCase(ThautTests, Test_23)
{
    testExecute("(if #f (if #t 1 2) (if #f 3 4))", "4");
}

LCPP_TestCase(ThautTests, Test_24)
{
}

LCPP_TestCase(ThautTests, Test_25)
{
    testExecute("(define (add x y) (+ x y)) (define (test) (add 2 3)) (test)", "5");
    testExecute("(define (add x y) (+ x y 1)) (test)", "6");
}

LCPP_TestCase(ThautTests, Test_26)
{
    testExecute("(define (sub1 x) (sub x 1)) (define (sub x y) (- x y)) (sub1 4)", "3");
}

LCPP_TestCase(ThautTests, Test_27)
{
    testExecute("(define consTest (cons 1 2))\nconsTest", "(1 . 2)");
    testExecute("(car consTest)", "1");
    testExecute("(cdr consTest)", "2");
}

LCPP_TestCase(ThautTests, Test_28)
{
}

LCPP_TestCase(ThautTests, Test_29)
{
}

LCPP_TestCase(ThautTests, Test_30)
{
    testExecute("(define (innerFuncTest) \n(define (innerFunc x y) (+ x y)) \n(innerFunc 2 3)) \n(innerFuncTest)", "5");
}

LCPP_TestCase(ThautTests, Test_31)
{
    testExecute("(define (innerForwardRef x)\n (define (fwd x) (fwd1 x))\n (define (fwd1 x) (+ x 1))\n (fwd x))\n (innerForwardRef 1)", "2");
}

LCPP_TestCase(ThautTests, Test_32)
{
    testExecute("'(1 2 3)", "(1 2 3)");
}

LCPP_TestCase(ThautTests, Test_33)
{
    testExecute("'symbol", "symbol");
}

LCPP_TestCase(ThautTests, Test_34)
{
}

LCPP_TestCase(ThautTests, Test_35)
{
    testExecute("(define var2 2) (set! var2 4) var2", "4");
    testExecute("(set! var2 10) var2", "10");
    testExecute("(set! var2 '(1 2 3)) var2", "(1 2 3)");
}

LCPP_TestCase(ThautTests, Test_36)
{
    testExecute("((lambda () (define x 2) (set! x 4) x))", "4");
}

LCPP_TestCase(ThautTests, Test_37)
{
}

LCPP_TestCase(ThautTests, Test_38)
{
    testExecute("((lambda () (define x 3) (set! x '(1 2 3)) x))", "(1 2 3)");
}

LCPP_TestCase(ThautTests, Test_39)
{
    testExecute("((lambda (x y)\n ((lambda () (+ x y))))\n 2 3)", "5");
}

LCPP_TestCase(ThautTests, Test_40)
{
    testExecute("((lambda (x)\n ((lambda () (+ x x))))\n 2)", "4");
}

LCPP_TestCase(ThautTests, Test_41)
{
    testExecute("(define (call2 func x y) (func x y))\n(call2 + 2 3)", "5");
}

LCPP_TestCase(ThautTests, Test_42)
{
    testExecute("(define (makeAdder x) (lambda (y) (+ x y)))\n((makeAdder 5) 2)", "7");
    testExecute("((makeAdder 4) 2)", "6");
    testExecute("(define (makeLazyAdd x adder) (lambda () (adder x)))\n((makeLazyAdd 3 (makeAdder 9)))", "12");
}

LCPP_TestCase(ThautTests, Test_43)
{
}

LCPP_TestCase(ThautTests, Test_44)
{
    testExecute("(define (innerAdder x) ((lambda () (lambda (y) (+ x y)))))\n((innerAdder 6) 5)", "11");
}

LCPP_TestCase(ThautTests, Test_45)
{
    testExecute("(define (bind func x) (lambda (y) (func x y)))\n((bind + 10) 11)", "21");
}

LCPP_TestCase(ThautTests, Test_46)
{
}

LCPP_TestCase(ThautTests, Test_47)
{
    testExecute("(eq? null null)", "#t");
}

LCPP_TestCase(ThautTests, Test_48)
{
    testExecute("(eq? #t #f)", "#f");
}

LCPP_TestCase(ThautTests, Test_49)
{
    testExecute("(eq? #t #t)", "#t");
}

LCPP_TestCase(ThautTests, Test_50)
{
    testExecute("(eq? 'test 'test)", "#t");
}

LCPP_TestCase(ThautTests, Test_51)
{
    testExecute("(eq? 'test 'blup)", "#f");
}

LCPP_TestCase(ThautTests, Test_52)
{
    testExecute("(eq? 1 1)", "#f");
}

LCPP_TestCase(ThautTests, Test_53)
{
    testExecute("(define (innerSameName pair)"
                "  (define (helper pair) (+ pair 1))"
                "  (+ (helper (car pair))\n(helper (car (cdr pair)))))"
                "(innerSameName (quote (4 5)))", "11");
}

LCPP_TestCase(ThautTests, Test_54)
{
  testExecute("(define (fac x)"
              "  (define (helper i sum)"
              "    (if (= i x)"
              "      (+ sum i)"
              "      (helper (+ i 1) (+ sum i))))"
              "  (helper 0 0))"
              "(fac 10)", "55");
}

LCPP_TestCase(ThautTests, Test_55)
{
    testExecute("(define (executeFile name) (eval (read (file.read-string name))))", "#v");
    testExecute("(executeFile \"prime.lisp\")", "#v");
    testExecute("(computePrimes 10)", "(29 23 19 17 13 11 7 5 3 2)");
}

LCPP_TestCase(ThautTests, Test_56)
{
}

LCPP_TestCase(ThautTests, Test_57)
{
    testExecute("(define (overflow x max)"
            "  (if (= x max)"
            "    x"
            "    (+ 1 (overflow (+ x 1) max))))"
            "(overflow 0 1000)", "2000");
}

LCPP_TestCase(ThautTests, Test_58)
{
}

LCPP_TestCase(ThautTests, Test_59)
{
    testExecute("((lambda () (define x 0) ((lambda () (set! x 111))) x))", "111");
}

LCPP_TestCase(ThautTests, Test_60)
{
    testExecute("((lambda () (define x 0) ((lambda () (set! x 123) (set! x (+ x 1)))) (set! x (+ x 1)) x))", "125");
}

LCPP_TestCase(ThautTests, Test_61)
{
    testExecute("((lambda () (define x 0) ((lambda () ((lambda () (set! x 12))) x)) ))", "12");
}

LCPP_TestCase(ThautTests, Test_62)
{
    evalString("(define (executeFile name) (eval (read (file.read-string name))))");
    testExecute("(executeFile \"oopTest.lisp\")", "#t"); // The last return value within the file.
}

LCPP_TestCase(ThautTests, Test_63)
{
    evalString("(define (executeFile name) (eval (read (file.read-string name))))");
    testExecute("(executeFile \"stdlib.lisp\")", "#t");
    testExecute("(executeFile \"stdlib.lisp\")", "#t"); // execute a second time to make sure reloading works
}

LCPP_TestCase(ThautTests, Test_64)
{
}

LCPP_TestCase(ThautTests, Test_65)
{
    evalString("(eval (read (file.read-string \"stdlib.lisp\")))");
    testExecute(
        "((lambda () "
        "  (filter "
        "    (iota 0 10 1) "
        "    ((lambda () "
        "      (define last 0) "
        "      (lambda (x) "
        "        (print (cons x last)) "
        "        (if (> x (* last 2)) "
        "          ((lambda () (set! last x) #t))"
        "          #f"
        "        )"
        "      ))))))", "(1 3 7)");
    testExecute("(filter (iota 0 10000 1) (lambda (x) (== (% x 1000) 0)))", "(0 1000 2000 3000 4000 5000 6000 7000 8000 9000)");
}

LCPP_TestCase(ThautTests, Test_66)
{
}

LCPP_TestCase(ThautTests, Test_67)
{
    evalString("(eval (read (file.read-string \"stdlib.lisp\")))");
    testExecute("(reduce (iota 0 1000 1) +)", "499500");
}

LCPP_TestCase(ThautTests, Test_68)
{
    evalString("(eval (read (file.read-string \"stdlib.lisp\")))");
    testExecute("(list-tail '(1 2 3 4) 2)", "(3 4)");
}

LCPP_TestCase(ThautTests, Test_69)
{
    testExecute("((lambda () (begin (define x 1) (set! x (+ x 2)) x)))", "3");
}

LCPP_TestCase(ThautTests, Test_70)
{
    testExecute("(define (test x) (if (> x 0) (begin 2 3) (begin 4 5))) (+ (test -1) (test 1))", "8");
}

LCPP_TestCase(ThautTests, Test_71)
{
    testExecute(
        "((lambda ()"
        "  (define x 3)"
        "  (begin"
        "    (define x 5)"
        "    (assert (= x 5))"
        "  )"
        "  (define y 7)"
        "  (assert (= y 7))"
        "  x"
        "))", "5");
}

LCPP_TestCase(ThautTests, Test_72)
{
    testExecute(
        "((lambda ()"
        "  (define (x) 3)"
        "  (begin "
        "    (define (x) 5)"
        "    (assert (= (x) 5))"
        "  )"
        "  (define (y) 7)"
        "  (assert (= (y) 7))"
        "  (x)"
        "))"
        , "5");
}

LCPP_TestCase(ThautTests, Test_73)
{
    testExecute(
        "(((lambda ()"
        "  (define x 2)"
        "  (lambda () x)"
        ")))", "2");
}

LCPP_TestCase(ThautTests, Test_74)
{
    testExecute(
        "((lambda ()"
        "  (define x 1)"
        "  (begin "
        "    (assert (= x 1))"
        "    (set! x 2)"
        "  )"
        "  x"
        "))", "2");
}

LCPP_TestCase(ThautTests, Test_75)
{
}

LCPP_TestCase(ThautTests, Test_76)
{
    testExecute(
        "((lambda () "
        "  (define fun 0)"
        "  (begin"
        "    (define x 5) "
        "    (set! fun (lambda () x))"
        "  )"
        "  (fun)"
        "))", "5");
}

LCPP_TestCase(ThautTests, Test_77)
{
    evalString("(eval (read (file.read-string \"stdlib.lisp\")))");
    testExecute(
        "((lambda () "
        "  (filter "
        "    (iota 0 10 1) "
        "    ((lambda () "
        "      (define last 0) "
        "      (lambda (x) "
        "        (print (cons x last)) "
        "        (if (> x (* last 2)) "
        "          (begin (set! last x) #t)"
        "          #f"
        "        )"
        "))))))", "(1 3 7)");
}

LCPP_TestCase(ThautTests, Test_78)
{
    testExecute(
        "((lambda (a)"
        "  (define (func1 x)"
        "    (if (> x a)"
        "      (begin"
        "        (print 0)"
        "        (func1 (- x 1))"
        "      )"
        "      0"
        "    )"
        "  )"
        "  (if (< a 0)"
        "    (print \" - \")"
        "    (print \" + \")"
        "  )"
        "  (func1 11)"
        ") 10)", "0");
}

LCPP_TestCase(ThautTests, Test_79)
{
    CUT_ASSERT.notImplemented("Not supporting call/cc yet.");
    testExecute(
        "(define i 0)"
        "(define (for start end func)"
        "  ((lambda ()"
        "    (define jmp null)"
        "    (call/cc (lambda (cont) (set! jmp cont)))"
        "    (print \"jmp: \") (print jmp) (print \"\\n\")"
        "    (if (< start end)"
        "      (begin "
        "        (func)"
        "        (set! start (+ start 1))"
        "        (jmp null)"
        "      )"
        "      ;else"
        "      null"
        "    )"
        "  ))"
        ")"
        "(for 0 10 (lambda () (print \"call/cc\\n\") (set! i (+ i 1))))"
        "i", "10");
}

LCPP_TestCase(ThautTests, Test_80)
{
    CUT_ASSERT.notImplemented("Not supporting call/cc yet.");
    testExecute(
        "(define (f return)"
        "  (return 2)"
        "  3"
        ")"
        "(vmTest (f (lambda (x) x)) 3)"
        "(call/cc f)", "2");
}

