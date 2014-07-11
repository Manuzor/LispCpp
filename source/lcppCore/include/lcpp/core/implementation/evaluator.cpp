#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/runtime.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::RecursiveEvaluator::RecursiveEvaluator(Ptr<SchemeRuntime> pRuntime) :
    m_pRuntime(pRuntime),
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

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<SchemeObject> pObject)
{
    EZ_ASSERT(m_evalLevel == 0,
              "Attempt to recursively call the evaluate function with the default environment (/global)! "
              "This is not allowed. Call the other evaluate function and supply an environment.");
    return evalulate(m_pRuntime->globalEnvironment(), pObject);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject)
{
    m_evalLevel++;
    LCPP_SCOPE_EXIT{ m_evalLevel--; };

    EZ_LOG_BLOCK("RecursiveEvaluator::evalulate");
    ezLog::VerboseDebugMessage("Environment name: %s", pEnv->qualifiedName().GetData());
    ezLog::VerboseDebugMessage("Evaluating object of type %s: %s",
                               pObject->type().name,
                               pObject->toString().GetData());

    if(pObject->is<SchemeSymbol>())
    {
        auto pKey = pObject.cast<SchemeSymbol>();
        Ptr<SchemeObject> pResult;
        auto exists = pEnv->get(pKey, pResult);
        if(!exists.IsSuccess())
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("No binding found for symbol '%s'.", pKey->value().GetData());
            ezLog::VerboseDebugMessage(messsage.GetData());
            throw exceptions::NoBindingFound(messsage.GetData());
        }
        return pResult;
    }

    if(!pObject->is<SchemeCons>())
    {
        ezLog::VerboseDebugMessage("Object evaluated to itself.");
        return pObject;
    }

    auto pBody = pObject.cast<SchemeCons>();
    // Evaluate car in case it is a symbol, cons, etc.
    auto pFuncObject = evalulate(pEnv, pBody->car());

    if(pFuncObject->is<SchemeSyntax>())
    {
        ezLog::VerboseDebugMessage("Executing syntax object: %s",
                                   pFuncObject->toString().GetData());
        auto pUnevaluatedArgList = pBody->cdr();
        return pFuncObject.cast<SchemeSyntax>()->call(m_pRuntime, pEnv, pUnevaluatedArgList);
    }
    
    if (!pFuncObject->is<SchemeFunction>())
    {
        ezStringBuilder message;
        message.AppendFormat("Failed to call object: Type %s: %s",
                                   pFuncObject->type().name,
                                   pFuncObject->toString().GetData());
        ezLog::VerboseDebugMessage(message.GetData());
        throw exceptions::InvalidSyntax(message.GetData());
    }

    auto pArgs = m_pRuntime->factory()->copy(pBody->cdr());

    if(!isNil(pArgs))
    {
        ezLog::VerboseDebugMessage("Evaluating arguments: %s",
                                   pArgs->toString().GetData());
        evaluateEach(pEnv, pArgs);
    }

    ezLog::VerboseDebugMessage("Calling object: Type %s: %s",
                               pFuncObject->type().name,
                               pFuncObject->toString().GetData());
    return pFuncObject.cast<SchemeFunction>()->call(pArgs);
}

void
lcpp::RecursiveEvaluator::evaluateEach(Ptr<Environment> pEnv, Ptr<SchemeCons> pCons)
{
    auto pToEval = pCons->car();
    auto pEvaluated = evalulate(pEnv, pToEval);
    pCons->car(pEvaluated);

    if (isNil(pCons->cdr())) { return; }

    evaluateEach(pEnv, pCons->cdr().cast<SchemeCons>());
}
