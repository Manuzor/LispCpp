#include "stdafx.h"
#include "lcpp/core/typeSystem.h"


lcpp::TypeFactory::TypeFactory(ezAllocatorBase* pAllocator) :
    m_pAllocator(pAllocator),
    m_symbols(pAllocator),
    m_integers(pAllocator)
{
}

lcpp::TypeFactory::~TypeFactory()
{
    m_symbols.Clear();
    m_integers.Clear();
}

lcpp::Ptr<lcpp::Environment>
lcpp::TypeFactory::createEnvironment(const ezString& name, Ptr<Environment> pParent)
{
    return LCPP_NEW(m_pAllocator, Environment)(name, pParent);
}

lcpp::Ptr<lcpp::SchemeInteger>
lcpp::TypeFactory::createInteger(SchemeInteger::Number_t value)
{
    Ptr<SchemeInteger> pResult;
    if(!m_integers.TryGetValue(value, pResult))
    {
        pResult = LCPP_NEW(m_pAllocator, SchemeInteger)(value);
        m_integers[value] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeNumber>
lcpp::TypeFactory::createNumber(SchemeNumber::Number_t value)
{
    return LCPP_NEW(m_pAllocator, SchemeNumber)(value);
}

lcpp::Ptr<lcpp::SchemeString>
lcpp::TypeFactory::createString(const ezString& str)
{
    return LCPP_NEW(m_pAllocator, SchemeString)(str);
}

lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    Ptr<SchemeSymbol> pResult;
    if (!m_symbols.TryGetValue(symbol, pResult))
    {
        pResult = LCPP_NEW(m_pAllocator, SchemeSymbol)(symbol);
        m_symbols[symbol] = pResult;
    }
    EZ_ASSERT(pResult, "The result should never be a nullptr!");

    return pResult;
}

lcpp::Ptr<lcpp::SchemeCons>
lcpp::TypeFactory::createCons(Ptr<SchemeObject> pCar, Ptr<SchemeObject> pCdr)
{
    return LCPP_NEW(m_pAllocator, SchemeCons)(pCar, pCdr);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createUserDefinedFunction(Ptr<Environment> pParentEnv,
                                             Ptr<SchemeObject> pArgNameList,
                                             Ptr<SchemeCons> pBody)
{
    return LCPP_NEW(m_pAllocator, SchemeFunctionUserDefined)(createEnvironment("", pParentEnv), pArgNameList, pBody);
}

lcpp::Ptr<lcpp::SchemeFunction>
lcpp::TypeFactory::createBuiltinFunction(const ezString& name,
                                         Ptr<Environment> pParentEnv,
                                         SchemeFunctionBuiltin::Executor executor)
{
    return LCPP_NEW(m_pAllocator, SchemeFunctionBuiltin)(name, createEnvironment("", pParentEnv), executor);
}
