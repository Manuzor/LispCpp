#include "stdafx.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem.h"

lcpp::RecursiveEvaluator::RecursiveEvaluator() :
    m_defaultFactory(),
    m_pFactory(&m_defaultFactory),
    m_env(Environment::createTopLevelInstance())
{
}

lcpp::RecursiveEvaluator::RecursiveEvaluator(const CInfo& cinfo) :
    m_defaultFactory(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_defaultFactory),
    m_env(Environment::createTopLevelInstance())
{
}

lcpp::RecursiveEvaluator::~RecursiveEvaluator()
{
    m_pFactory = nullptr;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::RecursiveEvaluator::evalulate(Ptr<SchemeObject> pObject)
{
    if(!pObject->is<SchemeCons>())
    {
        return pObject;
    }
    // TODO Implement eval of cons
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
