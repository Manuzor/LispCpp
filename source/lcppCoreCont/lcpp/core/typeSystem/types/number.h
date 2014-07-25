#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace number
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();
        LCPP_API_CORE_CONT Ptr<LispObject> create(Integer_t value);

        class NumberType
        {
        public:
            enum Enum
            {
                ENUM_MIN = -1,

                Invalid,

                Integer,
                Float,

                ENUM_MAX
            };
        };

        class Data
        {
        public:

            NumberType::Enum m_type;

            union
            {
                Integer_t m_integer;
                Float_t m_float;
            };
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/number.inl"
