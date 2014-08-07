#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/builtins/syntax_builtinFunctions.h"

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

LCPP_TestGroup(Syntax_BuiltinFunctions);

LCPP_TestCase(Syntax_BuiltinFunctions, begin)
{
    auto pResult = LCPP_pNil;

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("begin");
    CUT_ASSERT.isTrue(syntax::builtin::getName(pResult) == symbol::create("begin"));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin)");
    CUT_ASSERT.isTrue(isVoid(pResult));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin 1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 2);
}
