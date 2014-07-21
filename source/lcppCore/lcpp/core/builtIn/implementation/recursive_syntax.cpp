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
    defineHelper(Ptr<LispEnvironment> pEnv,
                 Ptr<LispObject> pArgs,
                 std::function<void(Ptr<LispEnvironment>, Ptr<LispSymbol>, Ptr<LispObject>)> envOp)
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
            auto pLambda = lcpp::syntax::lambda(pEnv, LispCons::create(pTheArgs->cdr(), pArgList->cdr()));

            envOp(pEnv, pSymbol, pLambda);

            // Give the new lambda its name.
            pLambda.cast<LispFunction>()->name(pSymbol);

            return LCPP_VOID;
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

        value = LispRuntime::instance()->evaluator()->evalulate(pEnv, value);

        envOp(pEnv, symbol, value);

        return LCPP_VOID;
    }
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::define(Ptr<LispEnvironment> pEnv,
                     Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::define");

    return defineHelper(pEnv, pArgs, [](Ptr<LispEnvironment> pEnv, Ptr<LispSymbol> pSymbol, Ptr<LispObject> pObject) {
        pEnv->add(pSymbol, pObject);
        if(pObject->is<LispFunction>())
        {
            auto pNameable = pObject.cast<LispFunction>();
            if(!pNameable->hasName())
            {
                pNameable->name(pSymbol);
            }
        }
    });
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::set(Ptr<LispEnvironment> pEnv,
                  Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::set");

    return defineHelper(pEnv, pArgs, [](Ptr<LispEnvironment> pEnv, Ptr<LispSymbol> pSymbol, Ptr<LispObject> pObject) {
        auto result = pEnv->set(pSymbol, pObject);

        if (!result.IsSuccess())
        {
            ezStringBuilder message;
            message.Format("Cannot set variable before its definition: %s", pSymbol->value().GetData());
            throw exceptions::InvalidOperation(message.GetData());
        }
    });
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::lambda(Ptr<LispEnvironment> pEnv,
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

    return LispFunction_UserDefined::create(pEnv, pArgNameList, pBodyList);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::if_(Ptr<LispEnvironment> pEnv,
                  Ptr<LispObject> pArgs)
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
        auto numArgs = ezUInt32(0);

        count(pArgList, numArgs);
        if(numArgs < 2 || numArgs > 3)
        {
            ezStringBuilder builder;
            builder.AppendFormat("Expected 2 or 3 argument(s), got %u.", numArgs);
            throw exceptions::InvalidInput(builder.GetData());
        }
    }

    auto expression = pArgList->car();
    pArgList = pArgList->cdr().cast<LispCons>();
    auto thenPart = pArgList->car();

    auto expressionResult = LispRuntime::instance()->evaluator()->evalulate(pEnv, expression);

    if (!expressionResult->is<LispBool>())
    {
        throw exceptions::InvalidInput("Given expression in 'if' does not evaluate to a boolean value!");
    }

    if(isTrue(expressionResult))
    {
        return LispRuntime::instance()->evaluator()->evalulate(pEnv, thenPart);
    }

    if(isNil(pArgList->cdr()))
    {
        return LCPP_VOID;
    }

    auto elsePart = pArgList->cdr().cast<LispCons>()->car();

    return LispRuntime::instance()->evaluator()->evalulate(pEnv, elsePart);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::and(Ptr<LispEnvironment> pEnv,
                  Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::and");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    Ptr<LispObject> pResult = LCPP_TRUE;

    while(!isNil(pArgs))
    {
        EZ_ASSERT(pArgs->is<LispCons>(), "Parser error?");
        auto pArg = pArgs.cast<LispCons>()->car();
        pArgs = pArgs.cast<LispCons>()->cdr();

        pResult = LispRuntime::instance()->evaluator()->evalulate(pEnv, pArg);

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
lcpp::syntax::or(Ptr<LispEnvironment> pEnv,
                 Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::or");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    Ptr<LispObject> pResult = LCPP_FALSE;

    while(!isNil(pArgs))
    {
        EZ_ASSERT(pArgs->is<LispCons>(), "Parser error?");
        auto pArg = pArgs.cast<LispCons>()->car();
        pArgs = pArgs.cast<LispCons>()->cdr();

        pResult = LispRuntime::instance()->evaluator()->evalulate(pEnv, pArg);

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

lcpp::Ptr<lcpp::LispObject> 
lcpp::syntax::assertion(Ptr<LispEnvironment> pEnv,
                        Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::assertion");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    auto argCount = ezUInt32();
    auto result = count(pArgs, argCount);

    EZ_ASSERT(result.IsSuccess(), "Invalid input.");

    if (argCount < 1 || argCount > 2)
    {
        ezStringBuilder message;
        message.AppendFormat("Expected at least 1 [2] argument, got %u.", argCount);
        throw exceptions::InvalidInput(message.GetData());
    }

    auto pArgList = pArgs.cast<LispCons>();

    auto pCondition = pArgList->car();

    auto pMessage = Ptr<LispString>();

    if(!isNil(pArgList->cdr()))
    {
        pArgList = pArgList->cdr().cast<LispCons>();
        auto pMessageObject = pArgList->car();

        if(!pMessageObject->is<LispString>())
        {
            auto exceptionMessage = ezStringBuilder();
            exceptionMessage.AppendFormat("Expected string type, got %s", pMessageObject->type().name);
            throw exceptions::InvalidInput(exceptionMessage.GetData());
        }

        pMessage = pMessageObject.cast<LispString>();
    }

    auto pEvaluatedCondition = LispRuntime::instance()->evaluator()->evalulate(pEnv, pCondition);

    if(isFalse(pEvaluatedCondition))
    {
        auto message = ezStringBuilder();
        message.AppendFormat("Expression: %s", pCondition->toString().GetData());
        if (pMessage)
        {
            message.AppendFormat("; Message: %s", pMessage->value().GetData());
        }
        throw exceptions::Assertion(message.GetData());
    }

    return LCPP_VOID;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::begin(Ptr<LispEnvironment> pEnv,
                    Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("syntax::assertion");
    ezLog::VerboseDebugMessage("env: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("args: %s", pArgs->toString().GetData());

    auto pResult = Ptr<LispObject>();

    if(isNil(pArgs))
    {
        return LCPP_VOID;
    }

    while(true)
    {
        EZ_ASSERT(pArgs->is<LispCons>(), "Invalid input.");
        auto pArgList = pArgs.cast<LispCons>();

        pResult = LispRuntime::instance()->evaluator()->evalulate(pEnv, pArgList->car());
        pArgs = pArgList->cdr();

        if(isNil(pArgs))
        {
            break;
        }
    }

    return pResult;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::syntax::envGetCurrent(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pArgs)
{
    return pEnv;
}
