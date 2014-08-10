#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace number
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfoInteger();
        LCPP_API_CORE_CONT const MetaInfo& metaInfoFloat();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Integer_t value);
        LCPP_API_CORE_CONT Ptr<LispObject> create(Float_t value);

        LCPP_API_CORE_CONT Integer_t getInteger(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT Float_t getFloat(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> negate(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT Ptr<LispObject> invert(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT Ptr<LispObject> isZero(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> add(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> subtract(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> multiply(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> divide(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);

        LCPP_API_CORE_CONT Ptr<LispObject> greaterThan(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> greaterThanOrEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> equal(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> lowerThan(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
        LCPP_API_CORE_CONT Ptr<LispObject> lowerThanOrEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs);
    }
}

#include "lcpp/core/typeSystem/types/impl/number.inl"
#include "lcpp/core/typeSystem/types/numberHelpers.h"
