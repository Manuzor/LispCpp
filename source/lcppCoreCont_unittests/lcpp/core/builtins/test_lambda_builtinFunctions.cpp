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

    CUT_ASSERT.notImplemented("Implement the '/' function using the '*' function, i.e.: (/ 1 2 3 4) => (/ 1 (* 2 3 4))");
}
