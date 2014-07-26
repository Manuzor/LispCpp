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
    }
}

#include "lcpp/core/typeSystem/types/impl/number.inl"
#include "lcpp/core/typeSystem/types/numberHelpers.h"
