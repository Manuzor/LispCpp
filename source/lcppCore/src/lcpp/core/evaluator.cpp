#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/builtIn/recursive_functions.h"
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
    setupEnvironment();
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
        return pBody->car().cast<SchemeSyntax>()->call(m_pRuntime, pEnv);
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

void
lcpp::RecursiveEvaluator::setupEnvironment()
{
    auto pEnv = m_pRuntime->globalEnvironment();
    // constants
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("#t"), SCHEME_TRUE_PTR);
    pEnv->add(m_pRuntime->factory()->createSymbol("#f"), SCHEME_FALSE_PTR);
    pEnv->add(m_pRuntime->factory()->createSymbol("null"), SCHEME_NIL_PTR);
    pEnv->add(m_pRuntime->factory()->createSymbol("nil"), SCHEME_NIL_PTR);
    pEnv->add(m_pRuntime->factory()->createSymbol("#v"), SCHEME_VOID_PTR);
    
    // Utility functions
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("exit"),
              m_pRuntime->factory()->createBuiltinFunction("exit", pEnv, &builtin::exit));
    pEnv->add(m_pRuntime->factory()->createSymbol("dump"),
              m_pRuntime->factory()->createBuiltinFunction("dump", pEnv, &builtin::dump));

    // REPL
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("read"),
              m_pRuntime->factory()->createBuiltinFunction("read", pEnv, &builtin::read));
    pEnv->add(m_pRuntime->factory()->createSymbol("eval"),
              m_pRuntime->factory()->createBuiltinFunction("eval", pEnv, &builtin::eval));
    pEnv->add(m_pRuntime->factory()->createSymbol("print"),
              m_pRuntime->factory()->createBuiltinFunction("print", pEnv, &builtin::print));

    // File handling
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("file-open"),
              m_pRuntime->factory()->createBuiltinFunction("file-open", pEnv, &builtin::fileOpen));
    pEnv->add(m_pRuntime->factory()->createSymbol("file-is-open"),
              m_pRuntime->factory()->createBuiltinFunction("file-is-open", pEnv, &builtin::fileIsOpen));
    pEnv->add(m_pRuntime->factory()->createSymbol("file-read-string"),
              m_pRuntime->factory()->createBuiltinFunction("file-read-string", pEnv, &builtin::fileReadString));
    pEnv->add(m_pRuntime->factory()->createSymbol("file-close"),
              m_pRuntime->factory()->createBuiltinFunction("file-close", pEnv, &builtin::fileClose));

    // Basic math
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("+"),
              m_pRuntime->factory()->createBuiltinFunction("+", pEnv, &builtin::add));
    pEnv->add(m_pRuntime->factory()->createSymbol("-"),
              m_pRuntime->factory()->createBuiltinFunction("-", pEnv, &builtin::sub));
    pEnv->add(m_pRuntime->factory()->createSymbol("*"),
              m_pRuntime->factory()->createBuiltinFunction("*", pEnv, &builtin::mul));

    // Other
    //////////////////////////////////////////////////////////////////////////
    pEnv->add(m_pRuntime->factory()->createSymbol("set-recursion-limit"),
              m_pRuntime->factory()->createBuiltinFunction("set-recursion-limit", pEnv, &builtin::setRecursionLimit));
    pEnv->add(m_pRuntime->factory()->createSymbol("get-recursion-limit"),
              m_pRuntime->factory()->createBuiltinFunction("get-recursion-limit", pEnv, &builtin::getRecursionLimit));
}
