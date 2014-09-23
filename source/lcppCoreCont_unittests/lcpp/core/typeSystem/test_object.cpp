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
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/functionUtils/signature.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/file.h"
#include "lcpp/core/typeSystem/types/stream.h"

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
    ezStringBuilder streamContent;
    streamContent.AppendFormat("Hello world, this is the content of the stream.");

    auto pNil = LCPP_pNil;
    auto pTrue = LCPP_pTrue;
    auto pFalse = LCPP_pFalse;
    auto pVoid = LCPP_pVoid;

    auto pInteger = number::create(42);
    auto pFloat = number::create(3.1415f);

    auto pSymbol = symbol::create("hello-world");
    auto pString = str::create("this-is-a-string");
    auto pStream = stream::create(streamContent.GetIteratorFront());

    auto pCons = cons::create(pInteger, cons::create(cons::create(pFloat, pTrue), cons::create(pSymbol, cons::create(pString, LCPP_pNil))));
    auto pEnv = env::createTopLevel(pSymbol);
    auto pLambda = lambda::builtin::create(pEnv, &lambda::builtin::add, Signature::create(2, 42));
    auto pSyntax = syntax::builtin::create(&syntax::builtin::define);

    auto pFile = file::create();
    file::setFileName(pFile, pString);

    return;
}
