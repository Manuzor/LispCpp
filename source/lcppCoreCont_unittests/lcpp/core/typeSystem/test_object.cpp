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

    StackPtr<LispObject> pInteger = number::create(42);
    StackPtr<LispObject> pFloat = number::create(3.1415f);

    StackPtr<LispObject> pSymbol = symbol::create("hello-world");
    StackPtr<LispObject> pString = str::create("this-is-a-string");
    StackPtr<LispObject> pStream = stream::create(streamContent.GetIteratorFront());

    StackPtr<LispObject> pCons = cons::create(pInteger, cons::create(cons::create(pFloat, pTrue), cons::create(pSymbol, cons::create(pString, LCPP_pNil))));
    StackPtr<LispObject> pEnv = env::createTopLevel(pSymbol);
    StackPtr<LispObject> pLambda = lambda::builtin::create(pEnv, &lambda::builtin::add, Signature::create(2, 42));
    StackPtr<LispObject> pSyntax = syntax::builtin::create(&syntax::builtin::define);

    StackPtr<LispObject> pFile = file::create();
    file::setFileName(pFile, pString);
}
