#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace number
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfoInteger();
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfoFloat();

        LCPP_API_CORE_CONT StackPtr<LispObject> create(Integer_t value);
        LCPP_API_CORE_CONT StackPtr<LispObject> create(Float_t value);

        LCPP_API_CORE_CONT Integer_t getInteger(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT Float_t getFloat(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> negate(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT StackPtr<LispObject> invert(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT StackPtr<LispObject> isZero(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> add(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> subtract(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> multiply(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> divide(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> modulo(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);

        LCPP_API_CORE_CONT StackPtr<LispObject> greaterThan(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> greaterThanOrEqual(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> equal(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> lowerThan(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
        LCPP_API_CORE_CONT StackPtr<LispObject> lowerThanOrEqual(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);
    }
}

#include "lcpp/core/typeSystem/types/impl/number.inl"
#include "lcpp/core/typeSystem/types/numberHelpers.h"
