#include "stdafx.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/string.h"

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

LCPP_TestGroup(Syntax);

LCPP_TestCase(Syntax, Builtin)
{
    StackPtr<LispObject> pLambda = syntax::builtin::create(&testBuiltin);

    g_wasCalled = false;
    StackPtr<LispObject> pResult = LCPP_pFalse;

    {
        StackPtr<LispObject> pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        StackPtr<LispObject> pContCall = cont::create(pContMain, &object::call);
        cont::getStack(pContCall)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());
        cont::getStack(pContCall)->push(pLambda);

        cont::trampoline(pContCall);

        pResult = cont::getStack(pContMain)->get(-1);
    }

    CUT_ASSERT.isTrue(g_wasCalled);
    CUT_ASSERT.isTrue(!isFalse(pResult));
}

LCPP_TestCase(Syntax, Builtin_Name)
{
    StackPtr<LispObject> pSyntax = syntax::builtin::create(&testBuiltin);

    CUT_ASSERT.isFalse(syntax::builtin::hasName(pSyntax));
    CUT_ASSERT.isTrue(isNil(syntax::builtin::getName(pSyntax)));

    syntax::builtin::setName(pSyntax, symbol::create("my-syntax"));

    CUT_ASSERT.isTrue(syntax::builtin::hasName(pSyntax));
    CUT_ASSERT.isTrue(symbol::getValue(syntax::builtin::getName(pSyntax)).IsEqual("my-syntax"));
}

LCPP_TestCase(Syntax, Builtin_toString)
{
    StackPtr<LispObject> pSyntax = syntax::builtin::create(&testBuiltin);

    StackPtr<LispObject> pString = object::toString(pSyntax);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-syntax>"));

    syntax::builtin::setName(pSyntax, symbol::create("my-syntax"));

    pString = object::toString(pSyntax);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<builtin-syntax: my-syntax>"));
}

LCPP_TestCase(Syntax, UserDefined)
{
    StackPtr<LispObject> pArgList = LCPP_pNil;
    StackPtr<LispObject> pBody = cons::create(number::create(1337), LCPP_pNil);

    //StackPtr<LispObject> pLambda = syntax::create(LCPP_test_pRuntimeState->getGlobalEnvironment(), pArgList, pBody);
}
