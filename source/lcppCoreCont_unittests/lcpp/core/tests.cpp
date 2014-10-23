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
            CUT_ASSERT.notImplemented();
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
    testExecute("(define (add1 x) (add x 1)) (add1 3)", "4");
}

LCPP_TestCase(ThautTests, Test_15)
{
    testExecute("(if true 1 2)", "1");
}

LCPP_TestCase(ThautTests, Test_16)
{
    testExecute("(if false 1 2)", "2");
}

LCPP_TestCase(ThautTests, Test_17)
{
    testExecute("(if true (if true 1 2) 3)", "1");
}

LCPP_TestCase(ThautTests, Test_18)
{
    testExecute("(if true (if false 1 2) 3)", "2");
}

LCPP_TestCase(ThautTests, Test_19)
{
    testExecute("(if false (if false 1 2) 3)", "3");
}

LCPP_TestCase(ThautTests, Test_20)
{
    testExecute("(if true (if true 1 2) (if true 3 4))", "1");
}

LCPP_TestCase(ThautTests, Test_21)
{
    testExecute("(if true (if false 1 2) (if true 3 4))", "2");
}

LCPP_TestCase(ThautTests, Test_22)
{
    testExecute("(if false (if true 1 2) (if true 3 4))", "3");
}

LCPP_TestCase(ThautTests, Test_23)
{
    testExecute("(if false (if true 1 2) (if false 3 4))", "4");
}

LCPP_TestCase(ThautTests, Test_24)
{
    testExecute("(define (add x y) (+ x y)) (define (test) (add 2 3)) (test)", "5");
}

LCPP_TestCase(ThautTests, Test_25)
{
    testExecute("(define (add x y) (+ x y 1)) (test)", "6");
}

LCPP_TestCase(ThautTests, Test_26)
{
    testExecute("(define (sub1 x) (sub x 1)) (define (sub x y) (- x y)) (sub1 4)", "3");
}

LCPP_TestCase(ThautTests, Test_27)
{
    testExecute("(define consTest (cons 1 2))\nconsTest", "(1 . 2)");
}

LCPP_TestCase(ThautTests, Test_28)
{
    testExecute("(first consTest)", "1");
}

LCPP_TestCase(ThautTests, Test_29)
{
    testExecute("(rest consTest)", "2");
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
    testExecute("(define var2 2) (set! var2 4) var2", "4");
}

LCPP_TestCase(ThautTests, Test_35)
{
    testExecute("(set! var2 10) var2", "10");
}

LCPP_TestCase(ThautTests, Test_36)
{
    testExecute("((lambda () (define x 2) (set! x 4) x))", "4");
}

LCPP_TestCase(ThautTests, Test_37)
{
    testExecute("(set! var2 '(1 2 3)) var2", "(1 2 3)");
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
}

LCPP_TestCase(ThautTests, Test_43)
{
    testExecute("((makeAdder 4) 2)", "6");
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
    testExecute("(define (makeLazyAdd x adder) (lambda () (adder x)))\n((makeLazyAdd 3 (makeAdder 9)))", "12");
}

LCPP_TestCase(ThautTests, Test_47)
{
    testExecute("(eq? nil nil)", "#t");
}

LCPP_TestCase(ThautTests, Test_48)
{
    testExecute("(eq? true false)", "#f");
}

LCPP_TestCase(ThautTests, Test_49)
{
    testExecute("(eq? true true)", "#t");
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
    testExecute("(define (innerSameName pair)\n(define (helper pair) (+ pair 1))\n(+ (helper (first pair))\n(helper (first (rest pair)))))\n(innerSameName '(4 5))", "11");
}

LCPP_TestCase(ThautTests, Test_54)
{
    testExecute("(define (fac x)\n(define (helper i sum)\n(if (== i x)\n(+ sum i)\n(helper (+ i 1) (+ sum i))))\n(helper 0 0))\n(fac 10)", "55");
}

LCPP_TestCase(ThautTests, Test_55)
{
    testExecute("(executeFile \"prime.lisp\")", "#v");
}

LCPP_TestCase(ThautTests, Test_56)
{
    testExecute("(computePrimes 10)", "(29 23 19 17 13 11 7 5 3 2)");
}

LCPP_TestCase(ThautTests, Test_57)
{
    testExecute("(define (overflow x max)\n (if (== x max)\n x\n (+ 1 (overflow (+ x 1) max)))) (overflow 0 1000)", "2000");
}

LCPP_TestCase(ThautTests, Test_58)
{
    testExecute("(computePrimes 10)", "(29 23 19 17 13 11 7 5 3 2)");
}

LCPP_TestCase(ThautTests, Test_59)
{
    testExecute("((lambda () (define x 0) ((lambda () (set! x 111))) x))", "111");
}

LCPP_TestCase(ThautTests, Test_60)
{
    testExecute("((lambda () (define x 0) ((lambda () (set! x 123) (set! x (+ x 1)))) (set! x (+ x 1))))", "125");
}

LCPP_TestCase(ThautTests, Test_61)
{
    testExecute("((lambda () \n(define x 0) \n((lambda () \n((lambda () (set! x 12))) \nx)) \n))", "12");
}

LCPP_TestCase(ThautTests, Test_62)
{
    testExecute("(executeFile \"oopTest.lisp\")", "");
}

LCPP_TestCase(ThautTests, Test_63)
{
    testExecute("(executeFile \"stdlib.lisp\")", "");
}

LCPP_TestCase(ThautTests, Test_64)
{
    testExecute("(executeFile \"stdlib.lisp\")", ""); // execute a second time to make sure reloading works
}

LCPP_TestCase(ThautTests, Test_65)
{
    testExecute(
        "((lambda () "
        "  (filter "
        "    (iota 0 10 1) "
        "    ((lambda () "
        "      (define last 0) "
        "      (lambda (x) "
        "        (print (cons x last)) "
        "        (if (> x (* last 2)) "
        "          ((lambda () (set! last x) true))"
        "          false"
        "        )"
        "      ))))))", "(1 3 7)");
}

LCPP_TestCase(ThautTests, Test_66)
{
    testExecute("(filter (iota 0 10000 1) (lambda (x) (== (% x 1000) 0)))", "(0 1000 2000 3000 4000 5000 6000 7000 8000 9000)");
}

LCPP_TestCase(ThautTests, Test_67)
{
    testExecute("(reduce (iota 0 1000 1) +)", "499500");
}

LCPP_TestCase(ThautTests, Test_68)
{
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
        "    (vmTest x 5)"
        "  )"
        "  (define y 7)"
        "  (vmTest y 7)"
        "  x"
        "))", "3");
}

LCPP_TestCase(ThautTests, Test_72)
{
    testExecute(
        "((lambda ()"
        "  (define (x) 3)"
        "  (begin "
        "    (define (x) 5)"
        "    (vmTest (x) 5)"
        "  )"
        "  (define (y) 7)"
        "  (vmTest (y) 7)"
        "  (x)"
        "))"
        , "3");
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
        "    (vmTest x 1)"
        "    (set! x 2)"
        "  )"
        "  x"
        "))", "2");
}

LCPP_TestCase(ThautTests, Test_75)
{
    testExecute(
        "((lambda () "
        "  (define x 5) "
        "  ((lambda () (set! x 7)))"
        "  (vmTest x 7)"
        "  (begin "
        "    (define x 10) "
        "    (vmTest x 10)"
        "    ((lambda () (set! x 3)))"
        "    (vmTest x 3)"
        "  ) "
        "  (vmTest x 7)"
        "  ((lambda () (set! x 1)))"
        "  (vmTest x 1)"
        "  0"
        "))", "0");
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
    testExecute(
        "((lambda () "
        "  (filter "
        "    (iota 0 10 1) "
        "    ((lambda () "
        "      (define last 0) "
        "      (lambda (x) "
        "        (print (cons x last)) "
        "        (if (> x (* last 2)) "
        "          (begin (set! last x) true)"
        "          false"
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
        "    (display " - ")"
        "    (display " + ")"
        "  )"
        "  (func1 11)"
        ") 10)", "0");
}

LCPP_TestCase(ThautTests, Test_79)
{
    testExecute(
        "(define i 0)"
        "(define (for start end func)"
        "  ((lambda ()"
        "    (define jmp nil)"
        "    (call/cc (lambda (cont) (set! jmp cont)))"
        "    (display \"jmp: \") (print jmp) (display \"\n\")"
        "    (if (< start end)"
        "      (begin "
        "        (func)"
        "        (set! start (+ start 1))"
        "        (jmp nil)"
        "      )"
        "      ;else"
        "      nil"
        "    )"
        "  ))"
        ")"
        "(for 0 10 (lambda () (display \"call/cc\n\") (set! i (+ i 1))))"
        "i", "10");
}

LCPP_TestCase(ThautTests, Test_80)
{
    testExecute(
        "(define (f return)"
        "  (return 2)"
        "  3"
        ")"
        "(vmTest (f (lambda (x) x)) 3)"
        "(call/cc f)", "2");
}

