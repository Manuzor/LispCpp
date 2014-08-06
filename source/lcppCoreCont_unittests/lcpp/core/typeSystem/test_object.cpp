#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/metaInfo.h"

LCPP_TestGroup(Object);

LCPP_TestCase(Object, Basics)
{
    auto pObject = LCPP_pNil;

    CUT_ASSERT.isTrue(object::isType(pObject, Type::Nil));
    CUT_ASSERT.isTrue(object::getType(pObject) == Type(Type::Nil));
    CUT_ASSERT.isTrue(object::getMetaInfo(pObject).getType() == object::getType(pObject));
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
