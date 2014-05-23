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

lcpp::SchemeObject& lcpp::RecursiveEvaluator::evalulate(SchemeObject& object)
{
    if (!object.is<SchemeCons>())
    {
        return object;
    }
    // TODO Implement eval of cons
    return object;
}

lcpp::Environment& lcpp::RecursiveEvaluator::environment()
{
    return m_env;
}

const lcpp::Environment& lcpp::RecursiveEvaluator::environment() const
{
    return m_env;
}
