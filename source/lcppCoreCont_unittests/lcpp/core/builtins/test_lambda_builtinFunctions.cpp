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

LCPP_TestGroup(Lambda_BuiltinFunctions);

LCPP_TestCase(Lambda_BuiltinFunctions, add)
{
    auto pResult = LCPP_pNil;

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("+");
    CUT_ASSERT.isTrue(lambda::builtin::getName(pResult) == symbol::create("+"));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(+ 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 3);
}
