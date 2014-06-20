#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/builtIn/recursive_functions.h"
#include "lcpp/core/runtime.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator(Ptr<SchemeRuntime> pRuntime) :
    m_pRuntime(pRuntime)
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
    return evalulate(m_pRuntime->globalEnvironment(), pObject);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<Environment> pEnv, Ptr<SchemeObject> pObject)
{
    if(pObject->is<SchemeSymbol>())
    {
        auto key = pObject.cast<SchemeSymbol>();
        if(!pEnv->exists(key))
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("No binding found for symbol '%s'.", key->value().GetData());
            throw exceptions::InvalidInput(messsage.GetData());
        }
        Ptr<SchemeObject> pResult;
        pEnv->get(key, pResult);
        return pResult;
    }

    if(!pObject->is<SchemeCons>())
    {
        return pObject;
    }

    auto pBody = pObject.cast<SchemeCons>();
    auto pFuncObject = pBody->car();

    if(pBody->car()->is<SchemeSyntax>())
    {
        return pBody->car().cast<SchemeSyntax>()->call(m_pRuntime, pEnv);
    }
    else if(pBody->car()->is<SchemeCons>())
    {
        pFuncObject = evalulate(pEnv, pFuncObject);

        if(!pFuncObject->is<SchemeFunction>())
        {
            throw exceptions::InvalidInput("Attempt to call non-function object.");
        }
    }
    else if(pBody->car()->is<SchemeSymbol>())
    {
        auto pSymbol = pBody->car().cast<SchemeSymbol>();

        if(!pEnv->get(pSymbol, pFuncObject).IsSuccess())
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("No function binding found for symbol '%s'.", pSymbol->value().GetData());
            throw exceptions::InvalidInput(messsage.GetData());
        }

        if(!pFuncObject->is<SchemeFunction>())
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("Attempt to call non-function object '%s'.", pSymbol->value().GetData());
            throw exceptions::InvalidInput(messsage.GetData());
        }
    }
    else
    {
        throw exceptions::InvalidSyntax("Invalid expression cannot be called!");
    }

    auto pArgs = m_pRuntime->factory()->copy(pBody->cdr());

    if(!isNil(pArgs))
    {
        evaluateEach(pEnv, pArgs);
    }

    return pFuncObject.cast<SchemeFunction>()->call(m_pRuntime, pArgs);
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
}
