#include "stdafx.h"

#include "lcpp/core/builtIn/recursive_syntax.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/runtime.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

namespace lcpp
{
    Ptr<LispObject>
    defineHelper(Ptr<LispRuntime> pRuntime,
                 Ptr<Environment> pEnv,
                 Ptr<LispObject> pArgs,
                 std::function<void(Ptr<Environment>, Ptr<LispSymbol>, Ptr<LispObject>)> envOp)
    {
        EZ_LOG_BLOCK("syntax::defineHelper");
        ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
        ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

        if(isNil(pArgs))
        {
            throw exceptions::InvalidSyntax("Syntax 'define' expects exactly 2 arguments!");
        }

        auto pArgList = pArgs.cast<LispCons>();

        // Short-hand syntax for lambda definition
        if(pArgList->car()->is<LispCons>())
        {
            EZ_LOG_BLOCK("lambda short-hand syntax");

            auto pTheArgs = pArgList->car().cast<LispCons>();
            auto pSymbolObject = pTheArgs->car();
            if(!pSymbolObject->is<LispSymbol>())
            {
                throw exceptions::InvalidSyntax("First argument to lambda short-hand definition must be a symbol!");
            }
            auto pSymbol = pSymbolObject.cast<LispSymbol>();
            auto pLambda = lcpp::syntax::lambda(pRuntime, pEnv, pRuntime->factory()->createCons(pTheArgs->cdr(), pArgList->cdr()));

            envOp(pEnv, pSymbol, pLambda);

            // Give the new lambda its name.
            pLambda.cast<LispFunction>()->name(pSymbol->value());

            return SCHEME_VOID_PTR;
        }

        if(!pArgList->car()->is<LispSymbol>())
        {
            throw exceptions::InvalidSyntax("First argument to syntax 'define' must be a symbol!");
        }

        if(isNil(pArgList->cdr()))
        {
            throw exceptions::InvalidSyntax("Not enough arguments for syntax 'define'!");
        }

        if(!isNil(pArgList->cdr().cast<LispCons>()->cdr()))
        {
            throw exceptions::InvalidSyntax("Too many arguments for syntax 'define'!");
        }

        auto symbol = pArgList->car().cast<LispSymbol>();
        auto value = pArgList->cdr().cast<LispCons>()->car();

        value = pRuntime->evaluator()->evalulate(pEnv, value);

        envOp(pEnv, symbol, value);

        // If it is a function, give it its new name.
        if(value->is<LispFunction>())
        {
            value.cast<LispFunction>()->name(symbol->value());
        }

        return SCHEME_VOID_PTR;
    }
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::define(Ptr<LispRuntime> pRuntime,
                     Ptr<Environment> pEnv,
                     Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::define");

    return defineHelper(pRuntime, pEnv, pArgs, [](Ptr<Environment> pEnv, Ptr<LispSymbol> pSymbol, Ptr<LispObject> pObject) {
        pEnv->add(pSymbol, pObject);
        if(pObject->is<LispFunction>())
        {
            pObject.cast<LispFunction>()->name(pSymbol->value());
        }
    });
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::set(Ptr<LispRuntime> pRuntime,
                  Ptr<Environment> pEnv,
                  Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::set");

    return defineHelper(pRuntime, pEnv, pArgs, [](Ptr<Environment> pEnv, Ptr<LispSymbol> pSymbol, Ptr<LispObject> pObject) {
        auto result = pEnv->set(pSymbol, pObject);

        if (!result.IsSuccess())
        {
            ezStringBuilder message;
            message.Format("Cannot set variable before its definition: %s", pSymbol->value().GetData());
            throw exceptions::InvalidOperation(message.GetData());
        }
        
        if(pObject->is<LispFunction>())
        {
            pObject.cast<LispFunction>()->name(pSymbol->value());
        }
    });
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::lambda(Ptr<LispRuntime> pRuntime,
                     Ptr<Environment> pEnv,
                     Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::lambda");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    std::function<void(Ptr<LispCons>)> checkArgNameList = [&](Ptr<LispCons> pCons){
        if(!pCons->car()->is<LispSymbol>())
        {
            throw exceptions::InvalidSyntax("Lambda argument list only accepts symbols!");
        }
        if(isNil(pCons->cdr())) { return; }

        EZ_ASSERT(pCons->cdr()->is<LispCons>(), "Invalid AST!");

        checkArgNameList(pCons->cdr().cast<LispCons>());
    };

    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'lambda' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<LispCons>();
    auto pArgNameList = pArgList->car();

    // Arglist must be either nil or cons
    if(!isNil(pArgNameList) && !pArgNameList->is<LispCons>())
    {
        throw exceptions::InvalidSyntax("A 'lambda' needs an argument list!");
    }

    if(!isNil(pArgNameList))
    {
        checkArgNameList(pArgNameList.cast<LispCons>());
    }

    auto pBody = pArgList->cdr();

    if(isNil(pBody))
    {
        throw exceptions::InvalidSyntax("Lambda needs to have a body!");
    }

    auto pBodyList = pBody.cast<LispCons>();

    return pRuntime->factory()->createUserDefinedFunction(pEnv, pArgNameList, pBodyList);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::if_(Ptr<LispRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::if_");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());


    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'if' expects exactly 3 arguments!");
    }

    auto pArgList = pArgs.cast<LispCons>();

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
    auto thenPart = pArgList->cdr().cast<LispCons>()->car();
    auto elsePart = pArgList->cdr().cast<LispCons>()->cdr().cast<LispCons>()->car();

    auto expressionResult = pRuntime->evaluator()->evalulate(pEnv, expression);

    if (!expressionResult->is<LispBool>())
    {
        throw exceptions::InvalidInput("Given expression in 'if' does not evaluate to a boolean value!");
    }

    if(isTrue(expressionResult))
    {
        return pRuntime->evaluator()->evalulate(pEnv, thenPart);
    }

    return pRuntime->evaluator()->evalulate(pEnv, elsePart);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::and(Ptr<LispRuntime> pRuntime,
                  Ptr<Environment> pEnv,
                  Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::and");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    Ptr<LispObject> pResult = SCHEME_TRUE_PTR;

    while(!isNil(pArgs))
    {
        EZ_ASSERT(pArgs->is<LispCons>(), "Parser error?");
        auto pArg = pArgs.cast<LispCons>()->car();
        pArgs = pArgs.cast<LispCons>()->cdr();

        pResult = pRuntime->evaluator()->evalulate(pEnv, pArg);

        if (!pResult->is<LispBool>())
        {
            ezStringBuilder message;
            message.Format("Expected bool argument, got %s.", pResult->type().name);
            throw exceptions::InvalidInput(message.GetData());
        }
        
        if(isFalse(pResult))
        {
            break;
        }
    }

    return pResult;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::or(Ptr<LispRuntime> pRuntime,
                 Ptr<Environment> pEnv,
                 Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::or");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    Ptr<LispObject> pResult = SCHEME_FALSE_PTR;

    while(!isNil(pArgs))
    {
        EZ_ASSERT(pArgs->is<LispCons>(), "Parser error?");
        auto pArg = pArgs.cast<LispCons>()->car();
        pArgs = pArgs.cast<LispCons>()->cdr();

        pResult = pRuntime->evaluator()->evalulate(pEnv, pArg);

        if(!pResult->is<LispBool>())
        {
            ezStringBuilder message;
            message.Format("Expected bool argument, got %s.", pResult->type().name);
            throw exceptions::InvalidInput(message.GetData());
        }

        if(isTrue(pResult))
        {
            break;
        }
    }

    return pResult;
}
