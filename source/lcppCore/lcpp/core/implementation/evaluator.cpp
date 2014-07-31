#include "stdafx.h"

#include "lcpp/exceptions/exceptions.h"

#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/runtime.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::RecursiveEvaluator>
lcpp::RecursiveEvaluator::create()
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), RecursiveEvaluator)();
}

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_evalLevel(0)
{
}

lcpp::RecursiveEvaluator::~RecursiveEvaluator()
{
}

void
lcpp::RecursiveEvaluator::initialize()
{
}

lcpp::Ptr<lcpp::LispObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<LispObject> pObject)
{
    EZ_ASSERT(m_evalLevel == 0,
              "Attempt to recursively call the evaluate function with the default environment (/global)! "
              "This is not allowed. Call the other evaluate function and supply an environment.");
    return evalulate(LispRuntime::instance()->globalEnvironment(), pObject);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<LispEnvironment> pEnv, Ptr<LispObject> pObject)
{
    m_evalLevel++;
    LCPP_SCOPE_EXIT{ m_evalLevel--; };

    EZ_LOG_BLOCK("RecursiveEvaluator::evalulate");
    ezLog::VerboseDebugMessage("LispEnvironment name: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("Evaluating object of type %s: %s",
                               pObject->type().name,
                               pObject->toString().GetData());

    if(pObject->is<LispSymbol>())
    {
        auto pKey = pObject.cast<LispSymbol>();
        Ptr<LispObject> pResult;
        auto exists = pEnv->get(pKey, pResult);
        if(!exists.Succeeded())
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("No binding found for symbol '%s'.", pKey->value().GetData());
            ezLog::VerboseDebugMessage(messsage.GetData());
            throw exceptions::NoBindingFound(messsage.GetData());
        }
        return pResult;
    }

    if(!pObject->is<LispCons>())
    {
        ezLog::VerboseDebugMessage("Object evaluated to itself.");
        return pObject;
    }

    auto pBody = pObject.cast<LispCons>();
    // Evaluate car in case it is a symbol, cons, etc.
    auto pFuncObject = evalulate(pEnv, pBody->car());

    if(pFuncObject->is<LispSyntax>())
    {
        ezLog::VerboseDebugMessage("Executing syntax object: %s",
                                   pFuncObject->toString().GetData());
        auto pUnevaluatedArgList = pBody->cdr();
        return pFuncObject.cast<LispSyntax>()->call(pEnv, pUnevaluatedArgList);
    }

    if (!pFuncObject->is<LispFunction>())
    {
        ezStringBuilder message;
        message.AppendFormat("Failed to call object: Type %s: %s",
                                   pFuncObject->type().name,
                                   pFuncObject->toString().GetData());
        ezLog::VerboseDebugMessage(message.GetData());
        throw exceptions::InvalidSyntax(message.GetData());
    }

    auto pArgs = pBody->cdr()->copy();

    if(!isNil(pArgs))
    {
        ezLog::VerboseDebugMessage("Evaluating arguments: %s",
                                   pArgs->toString().GetData());
        evaluateEach(pEnv, pArgs);
    }

    ezLog::VerboseDebugMessage("Calling object: Type %s: %s",
                               pFuncObject->type().name,
                               pFuncObject->toString().GetData());
    return pFuncObject.cast<LispFunction>()->call(pArgs);
}

void
lcpp::RecursiveEvaluator::evaluateEach(Ptr<LispEnvironment> pEnv, Ptr<LispCons> pCons)
{
    auto pToEval = pCons->car();
    auto pEvaluated = evalulate(pEnv, pToEval);
    pCons->car(pEvaluated);

    if (isNil(pCons->cdr())) { return; }

    evaluateEach(pEnv, pCons->cdr().cast<LispCons>());
}
