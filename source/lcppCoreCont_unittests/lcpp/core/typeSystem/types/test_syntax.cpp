#include "stdafx.h"
#include "lcpp/core/typeSystem/objectUtils.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    static auto g_wasCalled = bool(false);

    static Ptr<LispObject> testBuiltin(Ptr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        CUT_ASSERT.isFalse(g_wasCalled);
        g_wasCalled = true;

        LCPP_cont_return(pCont, LCPP_pTrue);
    }
}

LCPP_TestGroup(Syntax);

LCPP_TestCase(Syntax, Builtin)
{
    auto pLambda = syntax::builtin::create(&testBuiltin);

    g_wasCalled = false;
    auto pResult = LCPP_pFalse;

    {
        auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        auto pContCall = cont::create(pContMain, &call);
        cont::getStack(pContCall)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
        cont::getStack(pContCall)->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(g_wasCalled);
    CUT_ASSERT.isTrue(!isFalse(pResult));
}

LCPP_TestCase(Syntax, Builtin_toString)
{
    auto pSyntax = syntax::builtin::create(&testBuiltin);

    auto pString = toString(pSyntax);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-syntax>"));
}

LCPP_TestCase(Syntax, UserDefined)
{
    auto pState = LCPP_test_pRuntimeState;

    auto pArgList = LCPP_pNil;
    auto pBody = cons::create(number::create(1337), LCPP_pNil);

    //auto pLambda = syntax::create(pState->getGlobalEnvironment(), pArgList, pBody);
}
