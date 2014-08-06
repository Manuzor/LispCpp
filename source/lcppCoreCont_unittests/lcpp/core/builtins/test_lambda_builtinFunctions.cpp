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

namespace lcpp
{
    static Ptr<LispObject> evalStream(Ptr<LispObject> pStream)
    {
        auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        auto pContEval = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEval)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());

        auto pContRead = cont::create(pContEval, &reader::read);
        cont::getStack(pContRead)->push(pStream);

        cont::trampoline(pContRead);

        return cont::getStack(pContMain)->get(-1);
    }

    static Ptr<LispObject> evalString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        return evalStream(pStream);
    }
}

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
