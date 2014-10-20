#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

#include "lcpp/core/functionUtils/signature.h"

#include "lcpp/core/typeSystem/types/lambda_builtin.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    static auto g_wasCalled = bool(false);

    static Ptr<LispObject> testBuiltin(StackPtr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        CUT_ASSERT.isFalse(g_wasCalled);
        g_wasCalled = true;

        LCPP_cont_return(pCont, LCPP_pTrue);
    }
}

LCPP_TestGroup(Lambda_Builtin);

LCPP_TestCase(Lambda_Builtin, Basics)
{
    StackPtr<LispObject> pLambda = lambda::builtin::create(LCPP_test_pRuntimeState->getGlobalEnvironment(),
                                                           &testBuiltin,
                                                           Signature::create(0, Signature::VarArg));

    g_wasCalled = false;
    StackPtr<LispObject> pResult = LCPP_pFalse;

    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        StackPtr<LispObject> pContCall = cont::create(pContMain, &object::call);
        cont::getStack(pContCall)->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(g_wasCalled);
    CUT_ASSERT.isTrue(!isFalse(pResult));
}

LCPP_TestCase(Lambda_Builtin, Name)
{
    StackPtr<LispObject> pEnv = LCPP_test_pRuntimeState->getGlobalEnvironment();

    StackPtr<LispObject> pLambda = lambda::builtin::create(pEnv, &testBuiltin, Signature::create(0, Signature::VarArg));

    CUT_ASSERT.isFalse(lambda::builtin::hasName(pLambda));
    CUT_ASSERT.isTrue(isNil(lambda::builtin::getName(pLambda)));

    lambda::builtin::setName(pLambda, symbol::create("my-lambda"));

    CUT_ASSERT.isTrue(lambda::builtin::hasName(pLambda));
    CUT_ASSERT.isTrue(symbol::getValue(lambda::builtin::getName(pLambda)).IsEqual("my-lambda"));
}

LCPP_TestCase(Lambda_Builtin, toString)
{
    StackPtr<LispObject> pEnv = LCPP_test_pRuntimeState->getGlobalEnvironment();

    StackPtr<LispObject> pLambda = lambda::builtin::create(pEnv, &testBuiltin, Signature::create(0, Signature::VarArg));

    StackPtr<LispObject> pString = object::toString(pLambda);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-procedure>"));

    lambda::builtin::setName(pLambda, symbol::create("this-is-a-name"));

    pString = object::toString(pLambda);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-procedure: this-is-a-name>"));
}
