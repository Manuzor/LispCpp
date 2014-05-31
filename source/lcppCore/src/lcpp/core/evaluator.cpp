#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/builtinFunctions_recursive.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory),
    m_syntax(Environment::createTopLevelInstance()),
    m_env("top", &m_syntax)
{
    setupSyntax();
}

lcpp::RecursiveEvaluator::RecursiveEvaluator(const CInfo& cinfo) :
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory),
    m_syntax(Environment::createTopLevelInstance()),
    m_env("top", &m_syntax)
{
    setupSyntax();
}

lcpp::RecursiveEvaluator::~RecursiveEvaluator()
{
    m_pFactory = nullptr;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<SchemeObject> pObject)
{
    return evalulate(&m_env, pObject);
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
    auto pSymbol = pBody->car().cast<SchemeSymbol>();

    Ptr<SchemeObject> pFuncObject;

    if(m_syntax.get(pSymbol, pFuncObject).IsSuccess())
    {
        EZ_ASSERT(pFuncObject->is<SchemeFunction>(), "Invalid scheme object in syntax environment.");
        return pFuncObject.cast<SchemeFunction>()->call(this, pBody->cdr());
    }

    if(!m_env.get(pSymbol, pFuncObject).IsSuccess())
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

    return pFuncObject.cast<SchemeFunction>()->call(this, pBody->cdr());
}

lcpp::Environment&
lcpp::RecursiveEvaluator::environment()
{
    return m_env;
}

const lcpp::Environment&
lcpp::RecursiveEvaluator::environment() const
{
    return m_env;
}

void lcpp::RecursiveEvaluator::setupSyntax()
{
    // TODO Fix std::bind not working here for some reason
    m_syntax.add(m_pFactory->createSymbol("define"), m_pFactory->createBuiltinFunction("define", &m_env,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::define(pEnv, pEval, pArgs);
    }));
    m_syntax.add(m_pFactory->createSymbol("exit"), m_pFactory->createBuiltinFunction("exit", &m_env,
        [](Ptr<Environment> pEnv, Ptr<IEvaluator> pEval, Ptr<SchemeObject> pArgs){
        return builtin::exit(pEnv, pEval, pArgs);
    }));
}
