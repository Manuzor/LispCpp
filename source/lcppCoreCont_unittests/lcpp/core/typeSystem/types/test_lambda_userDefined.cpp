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

LCPP_TestGroup(Lambda_UserDefined);

LCPP_TestCase(Lambda_UserDefined, Basics)
{
    StackPtr<LispObject> pArgList = cons::create(symbol::create("x"), LCPP_pNil);
    StackPtr<LispObject> pBodyList = cons::create(number::create(1337), LCPP_pNil);

    StackPtr<LispObject> pLambda = lambda::userDefined::create(LCPP_test_pRuntimeState->getGlobalEnvironment(), pArgList, pBodyList);

    StackPtr<LispObject> pResult = LCPP_pFalse;

    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        StackPtr<LispObject> pContCall = cont::create(pContMain, &object::call);
        cont::getStack(pContCall)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
        cont::getStack(pContCall)->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);
}

LCPP_TestCase(Lambda_UserDefined, toString)
{
    auto pState = LCPP_test_pRuntimeState;

    StackPtr<LispObject> pArgList = LCPP_pNil;
    StackPtr<LispObject> pBodyList = cons::create(number::create(1337), LCPP_pNil);

    StackPtr<LispObject> pLambda = lambda::userDefined::create(pState->getGlobalEnvironment(), pArgList, pBodyList);

    StackPtr<LispObject> pString = object::toString(pLambda);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<procedure>"));

    lambda::userDefined::setName(pLambda, symbol::create("this-is-the-name"));

    pString = object::toString(pLambda);

    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<procedure: this-is-the-name>"));
}

LCPP_TestCase(Lambda_UserDefined, uniqueCallerEnv)
{
    auto content = "(define (f x abort)           "
                   "    (if abort                 "
                   "        x                     "
                   "        (begin (f (- x 1)) x) "
                   "    )                         "
                   ")                             ";

    StackPtr<LispObject> pResult = LCPP_pNil;
    evalString(content);

    pResult = evalString("(f 42 #t)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42);

    pResult = evalString("(f 42 #f)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42, "Recursive call to 'f' modified the environment of the calling 'f'!");
}
