#include "stdafx.h"

#include "lcpp/core/builtIn/recursive_syntax.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/runtime.h"

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::define(Ptr<SchemeRuntime> pRuntime,
                     Ptr<Environment> pEnv,
                     Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'define' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();

    if(!pArgList->car()->is<SchemeSymbol>())
    {
        throw exceptions::InvalidSyntax("First argument to syntax 'define' must be a symbol!");
    }

    if(isNil(pArgList->cdr()))
    {
        throw exceptions::InvalidSyntax("Not enough arguments for syntax 'define'!");
    }

    if(!isNil(pArgList->cdr().cast<SchemeCons>()->cdr()))
    {
        throw exceptions::InvalidSyntax("Too many arguments for syntax 'define'!");
    }

    auto symbol = pArgList->car().cast<SchemeSymbol>();
    auto value = pArgList->cdr().cast<SchemeCons>()->car();

    value = pRuntime->evaluator()->evalulate(value);

    pEnv->add(symbol, value);

    // If it is a function, give it its new name.
    if(value->is<SchemeFunction>())
    {
        value.cast<SchemeFunction>()->name() = symbol->value();
    }

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::lambda(Ptr<SchemeRuntime> pRuntime,
                     Ptr<Environment> pEnv,
                     Ptr<SchemeObject> pArgs)
{
    std::function<void(Ptr<SchemeCons>)> checkArgNameList = [&](Ptr<SchemeCons> pCons){
        if(!pCons->car()->is<SchemeSymbol>())
        {
            throw exceptions::InvalidSyntax("Lambda argument list only accepts symbols!");
        }
        if(isNil(pCons->cdr())) { return; }

        EZ_ASSERT(pCons->cdr()->is<SchemeCons>(), "Invalid AST!");

        checkArgNameList(pCons->cdr().cast<SchemeCons>());
    };

    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'lambda' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();
    auto pArgNameList = pArgList->car();

    // Arglist must be either nil or cons
    if(!isNil(pArgNameList) && !pArgNameList->is<SchemeCons>())
    {
        throw exceptions::InvalidSyntax("A 'lambda' needs an argument list!");
    }

    if(!isNil(pArgNameList))
    {
        checkArgNameList(pArgNameList.cast<SchemeCons>());
    }

    auto pBody = pArgList->cdr();

    if(isNil(pBody))
    {
        throw exceptions::InvalidSyntax("Lambda needs to have a body!");
    }

    auto pBodyList = pBody.cast<SchemeCons>();

    return pRuntime->factory()->createUserDefinedFunction(pEnv, pArgNameList, pBodyList);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::if_(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'if' expects exactly 3 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();

    {
        ezUInt32 numArgs = 0;
        count(pArgList, numArgs);
        if(numArgs != 3)
        {
            ezStringBuilder builder;
            builder.AppendFormat("Expected 3 argument(s), got %u.", numArgs);
            throw exceptions::InvalidInput(builder.GetData());
        }
    }

    auto expression = pArgList->car();
    auto thenPart = pArgList->cdr().cast<SchemeCons>()->car();
    auto elsePart = pArgList->cdr().cast<SchemeCons>()->cdr().cast<SchemeCons>()->car();

    auto expressionResult = pRuntime->evaluator()->evalulate(expression);

    if (!expressionResult->is<SchemeBool>())
    {
        throw exceptions::InvalidInput("Given expression in 'if' does not evaluate to a boolean value!");
    }

    if(isTrue(expressionResult))
    {
        return pRuntime->evaluator()->evalulate(thenPart);
    }

    return pRuntime->evaluator()->evalulate(elsePart);
}
