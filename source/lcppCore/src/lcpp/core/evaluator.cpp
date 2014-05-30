#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory),
    m_env(Environment::createTopLevelInstance())
{
    setupSyntax();
}

lcpp::RecursiveEvaluator::RecursiveEvaluator(const CInfo& cinfo) :
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory),
    m_env(Environment::createTopLevelInstance())
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
    if(pObject->is<SchemeSymbol>())
    {
        auto key = pObject.cast<SchemeSymbol>();
        if (!m_env.exists(key))
        {
            ezStringBuilder messsage;
            messsage.AppendFormat("No binding found for symbol '%s'.", key->value().GetData());
            throw exceptions::InvalidInput(messsage.GetData());
        }
        Ptr<SchemeObject> pResult;
        m_env.get(key, pResult);
        return pResult;
    }

    if(!pObject->is<SchemeCons>())
    {
        return pObject;
    }

    auto pBody = pObject.cast<SchemeCons>();
    auto pSymbol = pBody->car().cast<SchemeSymbol>();

    Ptr<SchemeFunction> pFunc;

    if (m_syntax.TryGetValue(pSymbol->value(), pFunc))
    {
        return pFunc->call(pBody->cdr());
    }

    return pObject;
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
    m_syntax["define"] = m_pFactory->createFunction(&m_env, this,[&](Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs){
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
        auto value =  pArgList->cdr().cast<SchemeCons>()->car();

        value = evalulate(value);

        m_env.add(symbol, value);

        return SCHEME_VOID_PTR;
    });
    m_syntax["exit"] = m_pFactory->createFunction(&m_env, this, [&](Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs){
        ezInt32 status = 0;
        if(!isNil(pArgs) && pArgs->is<SchemeCons>() && pArgs.cast<SchemeCons>()->car()->is<SchemeInteger>())
        {
            auto tmp = pArgs.cast<SchemeCons>()->car().cast<SchemeInteger>()->value();
            status = ezInt32(tmp);
        }
        
        throw exceptions::Exit("Goodbye.", status);

        return SCHEME_VOID_PTR;
    });
}
