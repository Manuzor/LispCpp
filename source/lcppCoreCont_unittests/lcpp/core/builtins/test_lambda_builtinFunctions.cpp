#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"
#include "lcpp/core/typeSystem/types/nil.h"

#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/exceptions/arithmeticException.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/exceptions/typeCheckFailedException.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/void.h"

LCPP_TestGroup(Lambda_BuiltinFunctions);

LCPP_TestCase(Lambda_BuiltinFunctions, add)
{
    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(+)");
    CUT_ASSERT.isTrue(number::isZero(pResult));

    pResult = evalString("(+ 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 3);

    pResult = evalString("(+ 1 2.1415)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 3.1415);
}

LCPP_TestCase(Lambda_BuiltinFunctions, subtract)
{
    StackPtr<LispObject> pResult = LCPP_pNil;

    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(-)"); });

    pResult = evalString("(- 1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == -1);

    pResult = evalString("(- -1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(- 1.0)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == -1.0);

    pResult = evalString("(- 2 1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(- 1 2 3 4 5)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == -13);
}

LCPP_TestCase(Lambda_BuiltinFunctions, multiply)
{
    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(*)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(* 2 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 6);

    pResult = evalString("(* 2 1.5)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 3.0);
}

LCPP_TestCase(Lambda_BuiltinFunctions, divide)
{
    StackPtr<LispObject> pResult = LCPP_pNil;

    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(/)"); });

    pResult = evalString("(/ 1)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 1);

    pResult = evalString("(/ 2)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 0.5);

    CUT_ASSERT.throws<exceptions::DivisionByZero>([]{ evalString("(/ 0)"); });

    CUT_ASSERT.throws<exceptions::DivisionByZero>([]{ evalString("(/ 2 0)"); });

    pResult = evalString("(/ 1 2 3)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 1.0 / 2.0 / 3.0);
}

LCPP_TestCase(Lambda_BuiltinFunctions, modulo)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(%)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(% 1)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(% 1 2 3)"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(% 42 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 0);

    pResult = evalString("(% 43 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(% 44 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 2);

    pResult = evalString("(% 45 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 0);
}

LCPP_TestCase(Lambda_BuiltinFunctions, greaterThan)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(>)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(> 1)"); });

    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ evalString("(> 1 \"hello\")"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(> 1 2)");
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = evalString("(> 1 1)");
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = evalString("(> 1 0)");
    CUT_ASSERT.isTrue(isTrue(pResult));
}

LCPP_TestCase(Lambda_BuiltinFunctions, greaterThanOrEqual)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(>=)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(>= 1)"); });

    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ evalString("(>= 1 \"hello\")"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(>= 1 2)");
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = evalString("(>= 1 1)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(>= 1 0)");
    CUT_ASSERT.isTrue(isTrue(pResult));
}

LCPP_TestCase(Lambda_BuiltinFunctions, equal)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(=)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(= 1)"); });

    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ evalString("(= 1 \"hello\")"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(= 1 2)");
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = evalString("(= 1 1)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(= 1 0)");
    CUT_ASSERT.isTrue(isFalse(pResult));
}

LCPP_TestCase(Lambda_BuiltinFunctions, lowerThan)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(<)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(< 1)"); });

    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ evalString("(< 1 \"hello\")"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(< 1 2)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(< 1 1)");
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = evalString("(< 1 0)");
    CUT_ASSERT.isTrue(isFalse(pResult));
}

LCPP_TestCase(Lambda_BuiltinFunctions, lowerThanOrEqual)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(<=)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(<= 1)"); });

    CUT_ASSERT.throws<exceptions::TypeCheckFailed>([]{ evalString("(<= 1 \"hello\")"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(<= 1 2)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(<= 1 1)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(<= 1 0)");
    CUT_ASSERT.isTrue(isFalse(pResult));
}

LCPP_TestCase(Lambda_BuiltinFunctions, read)
{
    CUT_ASSERT.throwsNothing([]{ evalString("read"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(read \"(define x 1336) (define y (+ x 1)) 42\")");
    CUT_ASSERT.isTrue(object::isType(pResult, Type::Cons));
    StackPtr<LispObject> pName = syntax::builtin::getName(cons::getCar(pResult));
    const auto& nameValue = symbol::getValue(pName);
    CUT_ASSERT.isTrue(nameValue.IsEqual("begin"));

    pResult = evalObject(pResult);
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42);

    pResult = evalString("x");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1336);

    pResult = evalString("y");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);
}

LCPP_TestCase(Lambda_BuiltinFunctions, eval)
{
    CUT_ASSERT.throwsNothing([]{ evalString("eval"); });

    StackPtr<LispObject> pResult = LCPP_pNil;

    pResult = evalString("(eval (read \"(define x 42) 1337\"))");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);

    pResult = evalString("x");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42);
}

LCPP_TestCase(Lambda_BuiltinFunctions, print)
{
    CUT_ASSERT.throwsNothing([]{ evalString("print"); });

    auto pState = LCPP_test_pRuntimeState;
    auto pPrinterState = pState->getPrinterState();
    auto testStream = TestStringStream();
    pPrinterState->m_pOutStream = &testStream;

    StackPtr<LispObject> pResult = evalString("(print 42)");
    CUT_ASSERT.isTrue(isVoid(pResult));

    CUT_ASSERT.isTrue(testStream.m_content.IsEqual("42\n"));
}
