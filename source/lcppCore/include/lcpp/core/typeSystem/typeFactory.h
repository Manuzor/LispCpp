#pragma once
#include "lcpp/core/typeSystem/types/number.h"

namespace lcpp
{
    class SchemeObject;
    class SchemeCons;

    class LCPP_CORE_API TypeFactory
    {
    public:
        TypeFactory();

        SchemeInteger* createInteger(SchemeInteger::Number_t value);
        SchemeNumber*  createNumber(SchemeNumber::Number_t value);
        SchemeSymbol*  createSymbol(const ezString& symbol);
    };

}
