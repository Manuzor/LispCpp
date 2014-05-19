#include "stdafx.h"
#include "lcpp/core/typeSystem.h"


lcpp::TypeFactory::TypeFactory()
{
}

lcpp::TypeFactory::~TypeFactory()
{
    m_symbols.Clear();
    m_integers.Clear();
}

lcpp::SchemeInteger& lcpp::TypeFactory::createInteger(SchemeInteger::Number_t value)
{
    SchemeInteger* pResult = nullptr;
    if(!m_integers.TryGetValue(value, pResult))
    {
        pResult = new SchemeInteger(value);
        m_integers[value] = pResult;
    }

    return *pResult;
}

lcpp::SchemeNumber& lcpp::TypeFactory::createNumber(SchemeNumber::Number_t value)
{
    return *new SchemeNumber(value);
}

lcpp::SchemeString& lcpp::TypeFactory::createString(const ezString& str)
{
    return *new SchemeString(str);
}

lcpp::SchemeSymbol& lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    SchemeSymbol* pResult = nullptr;
    if (!m_symbols.TryGetValue(symbol, pResult))
    {
        pResult = new SchemeSymbol(symbol);
        m_symbols[symbol] = pResult;
    }

    return *pResult;
}

lcpp::SchemeCons& lcpp::TypeFactory::createCons(SchemeObject& car, SchemeObject& cdr)
{
    return *new SchemeCons(car, cdr);
}
