#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/exceptions/noBindingFoundException.h"
#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    static Ptr<LispObject> evalStream(Ptr<LispObject> pStream)
    {
        auto pContMain = cont::createTopLevel(LCPP_test_pRuntimeState);
        auto pContEval = cont::create(pContMain, &eval::evaluate);
        cont::getStack(pContEval)->push(LCPP_test_pRuntimeState->getGlobalEnvironment());

        auto pContRead = cont::create(pContEval, &reader::read);
        cont::getStack(pContRead)->push(pStream);

        cont::trampoline(pContRead);

        return cont::getStack(pContMain)->get(-1);
    }

    static Ptr<LispObject> evalString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        return evalStream(pStream);
    }
}

LCPP_TestGroup(Evaluator);

LCPP_TestCase(Evaluator, EvalEmptyOrWhitespaceString)
{
    auto pObject = Ptr<LispObject>();

    pObject = evalString("");
    CUT_ASSERT.isTrue(isVoid(pObject));

    pObject = evalString("    \n\t\r \t\t\n\r\r\n\r\n   \v\v  \n \t");
    CUT_ASSERT.isTrue(isVoid(pObject));
}

LCPP_TestCase(Evaluator, EvalSymbolInEnv)
{
    auto pObject = Ptr<LispObject>();

    env::addBinding(LCPP_test_pRuntimeState->getGlobalEnvironment(), symbol::create("x"), number::create(1337));

    pObject = evalString("x");
    CUT_ASSERT.isTrue(number::getInteger(pObject) == 1337);

    CUT_ASSERT.throws<exceptions::NoBindingFound>([&]{ evalString("this-is-a-symbol-that-should-never-exist."); });
}

LCPP_TestCase(Evaluator, EvalSyntax)
{
    auto pResult = LCPP_pNil;

    /// (quote ...)
    pResult = evalString(" quote");
    CUT_ASSERT.isTrue(pResult->isType(Type::Syntax));

    pResult = evalString("(  \n  quote     theSymbolX  \n\n\r\r\n  )   ");
    CUT_ASSERT.isTrue(symbol::getValue(pResult).IsEqual("theSymbolX"));

    /// (define <symbol> <value>)
    CUT_ASSERT.throws<exceptions::NoBindingFound>([&]{ evalString("x"); });

    pResult = evalString(" define");
    CUT_ASSERT.isTrue(pResult->isType(Type::Syntax));

    pResult = evalString("   (define x    1337 ) ");
    CUT_ASSERT.isTrue(isVoid(pResult));

    pResult = evalString("x");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);
}

LCPP_TestCase(Evaluator, define)
{
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(define)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(define x)"); });
    CUT_ASSERT.throws<exceptions::ArgumentCount>([]{ evalString("(define x 1 2)"); });
}
