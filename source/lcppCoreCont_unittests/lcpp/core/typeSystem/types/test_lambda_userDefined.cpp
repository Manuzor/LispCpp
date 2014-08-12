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
    auto pState = LCPP_test_pRuntimeState;

    auto pArgList = cons::create(symbol::create("x"), LCPP_pNil);
    auto pBodyList = cons::create(number::create(1337), LCPP_pNil);

    auto pLambda = lambda::userDefined::create(pState->getGlobalEnvironment(), pArgList, pBodyList);

    auto pResult = LCPP_pFalse;

    {
        auto pContMain = cont::createTopLevel(pState);
        auto pContCall = cont::create(pContMain, &object::call);
        auto pStackCall = cont::getStack(pContCall);
        pStackCall->push(pState->getGlobalEnvironment());
        pStackCall->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);
}

LCPP_TestCase(Lambda_UserDefined, toString)
{
    auto pState = LCPP_test_pRuntimeState;

    auto pArgList = LCPP_pNil;
    auto pBodyList = cons::create(number::create(1337), LCPP_pNil);

    auto pLambda = lambda::userDefined::create(pState->getGlobalEnvironment(), pArgList, pBodyList);

    auto pString = object::toString(pLambda);

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

    auto pResult = LCPP_pNil;
    evalString(content);

    pResult = evalString("(f 42 #t)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42);

    pResult = evalString("(f 42 #f)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42, "Recursive call to 'f' modified the environment of the calling 'f'!");
}
