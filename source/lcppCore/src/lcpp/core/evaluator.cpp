#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/builtinFunctions_recursive.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory),
    m_pSyntax(),
    m_pEnv()
{
}

lcpp::RecursiveEvaluator::RecursiveEvaluator(const CInfo& cinfo) :
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory),
    m_pSyntax(),
    m_pEnv()
{
}

lcpp::RecursiveEvaluator::~RecursiveEvaluator()
{
    m_pFactory = nullptr;
}

void lcpp::RecursiveEvaluator::initialize()
{
    m_pSyntax = m_pFactory->createEnvironment("", nullptr);
    m_pEnv = m_pFactory->createEnvironment("top", m_pSyntax);
    setupSyntax();
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

    if(evaluateSyntax(pObject, pObject).IsSuccess())
    {
        return pObject;
    }

    auto pBody = pObject.cast<SchemeCons>();

    // TODO add support for calling cons!
    if(!pBody->car()->is<SchemeSymbol>())
    {
        throw exceptions::InvalidSyntax("Expected symbol!");
    }

    auto pSymbol = pBody->car().cast<SchemeSymbol>();

    Ptr<SchemeObject> pFuncObject;

    if(!m_pEnv->get(pSymbol, pFuncObject).IsSuccess())
    {
        ezStringBuilder messsage;
        messsage.AppendFormat("No function binding found for symbol '%s'.", pSymbol->value().GetData());
        throw exceptions::InvalidInput(messsage.GetData());
    }

    if (!pFuncObject->is<SchemeFunction>())
    {
        ezStringBuilder messsage;
        messsage.AppendFormat("Attempt to call non-function object '%s'.", pSymbol->value().GetData());
        throw exceptions::InvalidInput(messsage.GetData());
    }

    auto pArgs = pBody->cdr();

    if(!isNil(pArgs))
    {
        evaluateEach(pEnv, pArgs);
    }

    return pFuncObject.cast<SchemeFunction>()->call(this, pArgs);
}

ezResult
lcpp::RecursiveEvaluator::evaluateSyntax(Ptr<SchemeObject> pObject, Ptr<SchemeObject>& out_pResult)
{
    if (!pObject->is<SchemeCons>())
    {
        return EZ_FAILURE;
    }

    auto pBody = pObject.cast<SchemeCons>();

    if(!pBody->car()->is<SchemeSymbol>())
    {
        throw exceptions::InvalidSyntax("Expected symbol!");
    }

    auto pSymbol = pBody->car().cast<SchemeSymbol>();

    Ptr<SchemeObject> pFuncObject;

    if(m_pSyntax->get(pSymbol, pFuncObject).IsSuccess())
    {
        EZ_ASSERT(pFuncObject->is<SchemeFunction>(), "Invalid scheme object in syntax environment.");
        out_pResult = pFuncObject.cast<SchemeFunction>()->call(this, pBody->cdr());
        return EZ_SUCCESS;
    }

    return EZ_FAILURE;
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

void lcpp::RecursiveEvaluator::evaluateEach(Ptr<Environment> pEnv, Ptr<SchemeCons> pCons)
{
    auto pToEval = pCons->car();
    auto pEvaluated = evalulate(pEnv, pToEval);
    pCons->car(pEvaluated);

    if (isNil(pCons->cdr())) { return; }

    evaluateEach(pEnv, pCons->cdr().cast<SchemeCons>());
}

void lcpp::RecursiveEvaluator::setupSyntax()
{
    // TODO Fix std::bind not working here for some reason
    m_pSyntax->add(m_pFactory->createSymbol("define"), m_pFactory->createBuiltinFunction("syntax:define", m_pEnv,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::define(pEnv, pEval, pArgs);
    }));
    m_pSyntax->add(m_pFactory->createSymbol("lambda"), m_pFactory->createBuiltinFunction("syntax:lambda", m_pEnv,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::lambda(pEnv, pEval, pArgs);
    }));
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
