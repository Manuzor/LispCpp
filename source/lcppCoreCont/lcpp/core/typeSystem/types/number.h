#pragma once

namespace lcpp
{
    class LispObject;

    namespace number
    {
        LCPP_API_CORE_CONT Ptr<LispObject> create(Integer_t value);

        class Type
        {
        public:
            enum Enum
            {
                ENUM_MIN = -1,

                Integer,
                Float,

                ENUM_MAX
            };
        };

        class Data
        {
        public:

            Data(Integer_t value);

            Data(Float_t value);

        private:

            Type::Enum m_type;

            union
            {
                Integer_t m_integer;
                Float_t m_float;
            };
        };
    }
}
