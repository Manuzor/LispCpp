#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/builtinFunctions_recursive.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory),
    m_pEnv()
{
}

lcpp::RecursiveEvaluator::RecursiveEvaluator(const CInfo& cinfo) :
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory),
    m_pEnv()
{
}

lcpp::RecursiveEvaluator::~RecursiveEvaluator()
{
}

void lcpp::RecursiveEvaluator::initialize()
{
    m_pEnv = m_pFactory->createEnvironment("", nullptr);
    setupEnvironment();
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<SchemeObject> pObject)
{
    return evalulate(m_pEnv, pObject);
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
        return pBody->car().cast<SchemeSyntax>()->call(pEnv, this);
    }
    else if(pBody->car()->is<SchemeCons>())
    {
        pFuncObject = evalulate(pFuncObject);

        if(!pFuncObject->is<SchemeFunction>())
        {
            throw exceptions::InvalidInput("Attempt to call non-function object.");
        }
    }
    else if(pBody->car()->is<SchemeSymbol>())
    {
        auto pSymbol = pBody->car().cast<SchemeSymbol>();

        if(!m_pEnv->get(pSymbol, pFuncObject).IsSuccess())
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

    auto pArgs = m_pFactory->copy(pBody->cdr());

    if(!isNil(pArgs))
    {
        evaluateEach(pEnv, pArgs);
    }

    return pFuncObject.cast<SchemeFunction>()->call(this, pArgs);
}

void lcpp::RecursiveEvaluator::evaluateEach(Ptr<Environment> pEnv, Ptr<SchemeCons> pCons)
{
    auto pToEval = pCons->car();
    auto pEvaluated = evalulate(pEnv, pToEval);
    pCons->car(pEvaluated);

    if (isNil(pCons->cdr())) { return; }

    evaluateEach(pEnv, pCons->cdr().cast<SchemeCons>());
}

lcpp::Ptr<lcpp::Environment>
lcpp::RecursiveEvaluator::environment()
{
    return m_pEnv;
}

lcpp::Ptr<const lcpp::Environment>
lcpp::RecursiveEvaluator::environment() const
{
    return m_pEnv;
}

lcpp::Ptr<lcpp::TypeFactory>
lcpp::RecursiveEvaluator::factory()
{
    return m_pFactory;
}

lcpp::Ptr<const lcpp::TypeFactory>
lcpp::RecursiveEvaluator::factory() const
{
    return m_pFactory;
}

void lcpp::RecursiveEvaluator::setupEnvironment()
{
    m_pEnv->add(m_pFactory->createSymbol("exit"), m_pFactory->createBuiltinFunction("exit", m_pEnv,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::exit(pEnv, pEval, pArgs);
    }));
    m_pEnv->add(m_pFactory->createSymbol("dump"), m_pFactory->createBuiltinFunction("dump", m_pEnv,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::dump(pEnv, pEval, pArgs);
    }));
    m_pEnv->add(m_pFactory->createSymbol("+"), m_pFactory->createBuiltinFunction("+", m_pEnv,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::add(pEnv, pEval, pArgs);
    }));
}
