#include "stdafx.h"

#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"

#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/object.h"

#include "lcpp/core/exceptions/noBindingFoundException.h"
#include "lcpp/core/typeSystem/types/bool.h"

LCPP_TestGroup(Syntax_BuiltinFunctions);

LCPP_TestCase(Syntax_BuiltinFunctions, if_)
{
    evalString("(if #t (define evaluated-to-true-1 #t) (define evaluated-to-false-1 #f))");

    CUT_ASSERT.throwsNothing([]{ evalString("evaluated-to-true-1"); });
    CUT_ASSERT.throws<exceptions::NoBindingFound>([]{ evalString("evaluated-to-false-1"); });

    //////////////////////////////////////////////////////////////////////////

    evalString("(if #f (define evaluated-to-true-2 #t) (define evaluated-to-false-2 #f))");

    CUT_ASSERT.throwsNothing([]{ evalString("evaluated-to-false-2"); });
    CUT_ASSERT.throws<exceptions::NoBindingFound>([]{ evalString("evaluated-to-true-2"); });
}

LCPP_TestCase(Syntax_BuiltinFunctions, define)
{
    auto pResult = LCPP_pNil;

    pResult = evalString("(define x 1)");

    CUT_ASSERT.isTrue(isVoid(pResult));

    pResult = evalString("x");

    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);
}

LCPP_TestCase(Syntax_BuiltinFunctions, define_name)
{
    auto pResult = evalString("(define x (lambda () 42))");
    pResult = evalString("x");
    
    CUT_ASSERT.isTrue(object::hasName(pResult));
    CUT_ASSERT.isTrue(symbol::getValue(object::getName(pResult)).IsEqual("x"));
}

LCPP_TestCase(Syntax_BuiltinFunctions, define_shortHandLambdaSyntax)
{
    auto pResult = LCPP_pNil;

    evalString("(define (the-answer) 42)");
    pResult = evalString("(the-answer)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 42);

    evalString("(define (something a b) 1 2 3 b a)");
    pResult = evalString("(something 5 4)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 5);
}

LCPP_TestCase(Syntax_BuiltinFunctions, begin)
{
    auto pResult = LCPP_pNil;

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("begin");
    CUT_ASSERT.isTrue(syntax::builtin::getName(pResult) == symbol::create("begin"));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin)");
    CUT_ASSERT.isTrue(isVoid(pResult));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin 1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(begin 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 2);
}

LCPP_TestCase(Syntax_BuiltinFunctions, lambda)
{
    auto pResult = LCPP_pNil;

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("lambda");
    CUT_ASSERT.isTrue(syntax::builtin::getName(pResult) == symbol::create("lambda"));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("(lambda () 1)");
    CUT_ASSERT.isTrue(isNil(lambda::userDefined::getName(pResult)));

    //////////////////////////////////////////////////////////////////////////

    pResult = evalString("((lambda () 1))");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);
}

LCPP_TestCase(Syntax_BuiltinFunctions, and)
{
    auto pResult = LCPP_pNil;

    pResult = evalString("(and)");
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = evalString("(and 1)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1);

    pResult = evalString("(and 1 2)");
    CUT_ASSERT.isTrue(number::getInteger(pResult) == 2);

    pResult = evalString("(and 1 #f 2)");
    CUT_ASSERT.isTrue(isFalse(pResult));
}
