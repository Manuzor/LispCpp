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

LCPP_TestGroup(Lambda_BuiltinFunctions);

LCPP_TestCase(Lambda_BuiltinFunctions, add)
{
    auto pResult = LCPP_pNil;

    pResult = evalString("(+)");
    CUT_ASSERT.isTrue(number::isZero(pResult));

    pResult = evalString("(+ 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 3);

    pResult = evalString("(+ 1 2.1415)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 3.1415);
}

LCPP_TestCase(Lambda_BuiltinFunctions, subtract)
{
    auto pResult = LCPP_pNil;

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
    auto pResult = LCPP_pNil;

    pResult = evalString("(*)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(* 2 3)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 6);

    pResult = evalString("(* 2 1.5)");
    CUT_ASSERT.isTrue(number::getFloat(pResult) == 3.0);
}

LCPP_TestCase(Lambda_BuiltinFunctions, divide)
{
    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

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

    auto pResult = LCPP_pNil;

    pResult = evalString("(<= 1 2)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(<= 1 1)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(<= 1 0)");
    CUT_ASSERT.isTrue(isFalse(pResult));
}
