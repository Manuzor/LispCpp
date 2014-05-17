#include "stdafx.h"
#include "lcpp/core/typeSystem.h"


lcpp::TypeFactory::TypeFactory()
{
}

lcpp::SchemeInteger* lcpp::TypeFactory::createInteger(SchemeInteger::Number_t value)
{
    return new SchemeInteger(value);
}

lcpp::SchemeNumber* lcpp::TypeFactory::createNumber(SchemeNumber::Number_t value)
{
    return new SchemeNumber(value);
}

lcpp::SchemeSymbol* lcpp::TypeFactory::createSymbol(const ezString& symbol)
{
    return new SchemeSymbol(symbol);
}
