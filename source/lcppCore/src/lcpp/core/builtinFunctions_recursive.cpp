#include "stdafx.h"

#include "lcpp/core/builtinFunctions_recursive.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/evaluator.h"

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::define(Ptr<Environment> pEnv,
                      Ptr<IEvaluator> pEvaluator,
                      Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Built-in function 'define' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();

    if(!pArgList->car()->is<SchemeSymbol>())
    {
        throw exceptions::InvalidSyntax("First argument to built-in function 'define' must be a symbol!");
    }

    if(isNil(pArgList->cdr()))
    {
        throw exceptions::InvalidSyntax("Not enough arguments for builtin-in 'define'!");
    }

    if(!isNil(pArgList->cdr().cast<SchemeCons>()->cdr()))
    {
        throw exceptions::InvalidSyntax("Too many arguments for built-in 'define'!");
    }

    auto symbol = pArgList->car().cast<SchemeSymbol>();
    auto value = pArgList->cdr().cast<SchemeCons>()->car();

    value = pEvaluator->evalulate(value);

    // Since pEnv is always the environment of the SchemeFunction that wraps 'define', we need to add the definition to the parent.
    EZ_ASSERT(pEnv->parent(), "Environment of 'define' must have a parent!");
    pEnv->parent()->add(symbol, value);

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::exit(Ptr<Environment> pEnv,
                    Ptr<IEvaluator> pEvaluator,
                    Ptr<SchemeObject> pArgs)
{
    ezInt32 status = 0;
    if(!isNil(pArgs) && pArgs->is<SchemeCons>() && pArgs.cast<SchemeCons>()->car()->is<SchemeInteger>())
    {
        auto tmp = pArgs.cast<SchemeCons>()->car().cast<SchemeInteger>()->value();
        status = ezInt32(tmp);
    }

    throw exceptions::Exit("Goodbye.", status);

    return SCHEME_VOID_PTR;
}

