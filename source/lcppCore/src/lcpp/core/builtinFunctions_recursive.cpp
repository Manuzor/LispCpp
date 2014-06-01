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

    // If it is a function, give it its new name.
    if(value->is<SchemeFunction>())
    {
        value.cast<SchemeFunction>()->name() = symbol->value();
    }

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::lambda(Ptr<Environment> pEnv,
                      Ptr<IEvaluator> pEvaluator,
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
        throw exceptions::InvalidSyntax("Built-in function 'define' expects exactly 2 arguments!");
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

    return pEvaluator->factory()->createUserDefinedFunction(pEnv->parent(), pArgNameList, pBodyList);
}

//////////////////////////////////////////////////////////////////////////

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

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::dump(Ptr<Environment> pEnv,
                    Ptr<IEvaluator> pEvaluator,
                    Ptr<SchemeObject> pArgs)
{
    if (isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected 1 argument, got none.");
    }
    
    auto pArgList = pArgs.cast<SchemeCons>();

    if(!isNil(pArgList->cdr()))
    {
        ezUInt32 numArgs = 0;
        count(pArgList, numArgs);
        ezStringBuilder builder;
        builder.AppendFormat("Expected 1 argument, got %u.", numArgs);
        throw exceptions::InvalidInput(builder.GetData());
    }

    auto pToDump = pArgList->car();

    if(pToDump->is<SchemeFunction>())
    {
        return pEvaluator->factory()->createString(pToDump.cast<SchemeFunction>()->dump());
    }

    return pEvaluator->factory()->createString(pToDump->toString());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::add(Ptr<Environment> pEnv,
                   Ptr<IEvaluator> pEvaluator,
                   Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 1 argument, got none.");
    }

    auto pArgList = pArgs.cast<SchemeCons>();
    SchemeInteger::Number_t iResult = 0;
    SchemeNumber::Number_t nResult = 0;

    bool integerOnly = true;

    while(true)
    {
        auto pArg = pArgList->car();
        
        if (pArg->is<SchemeInteger>())
        {
            iResult += pArg.cast<SchemeInteger>()->value();
        }
        else if (pArg->is<SchemeNumber>())
        {
            integerOnly = false;
            nResult += pArg.cast<SchemeNumber>()->value();
        }
        else
        {
            throw exceptions::InvalidInput("Invalid argument type for 'add'");
        }

        if(isNil(pArgList->cdr())) { break; }
        
        pArgList = pArgList->cdr().cast<SchemeCons>();
    }

    if (integerOnly)
    {
        return pEvaluator->factory()->createInteger(iResult);
    }

    return pEvaluator->factory()->createNumber(iResult + nResult);
}
