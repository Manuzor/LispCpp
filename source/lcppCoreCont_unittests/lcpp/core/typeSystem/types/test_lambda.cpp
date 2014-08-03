#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
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

LCPP_TestGroup(Lambda);

LCPP_TestCase(Lambda, Builtin)
{
    auto pState = LCPP_test_pRuntimeState;

    auto pLambda = lambda::builtin::create(pState->getGlobalEnvironment(), &testBuiltin);

    g_wasCalled = false;
    auto pResult = LCPP_pFalse;

    {
        auto pContMain = cont::createTopLevel(pState);
        auto pContCall = cont::create(pContMain, &call);
        cont::getStack(pContCall)->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(g_wasCalled);
    CUT_ASSERT.isTrue(!isFalse(pResult));
}

LCPP_TestCase(Lambda, Builtin_toString)
{
    auto pState = LCPP_test_pRuntimeState;
    auto pEnv = pState->getGlobalEnvironment();

    auto pLambda = lambda::builtin::create(pEnv, &testBuiltin);

    auto pString = toString(pLambda);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-procedure>"));
}

LCPP_TestCase(Lambda, UserDefined)
{
    auto pState = LCPP_test_pRuntimeState;

    auto pArgList = LCPP_pNil;
    auto pBody = cons::create(number::create(1337), LCPP_pNil);

    //auto pLambda = lambda::userDefined::create(pState->getGlobalEnvironment(), pArgList, pBody);
}
