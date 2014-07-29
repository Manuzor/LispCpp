#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TestGroup(Object);

LCPP_TestCase(Object, Basics)
{
    auto pObject = LCPP_pNil;

    CUT_ASSERT.isTrue(pObject->isType(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getType() == Type(Type::Nil));
    CUT_ASSERT.isTrue(pObject->getMetaInfo().getType() == pObject->getType());
}

LCPP_TestCase(Object, AllTypes)
{
    auto pNil = LCPP_pNil;
    auto pTrue = LCPP_pTrue;
    auto pFalse = LCPP_pFalse;
    auto pVoid = LCPP_pVoid;

    auto pInteger = number::create(42);
    auto pFloat = number::create(3.1415f);

    auto pSymbol = symbol::create("hello-world");
    // auto pString = string::create("this-is-a-string");

    // auto pCons = cons::create(pInteger, pNil);
    // auto pLambda = lambda::createBuiltin(pEnv, &lambda::builtin::myLambda);
    // auto pSyntax = syntax::createBuiltin(&syntax::builtin::mySyntax);
    auto pEnv = env::createTopLevel(pSymbol);

    // auto pFile = file::create("the-file-name.txt");

    return;
}
