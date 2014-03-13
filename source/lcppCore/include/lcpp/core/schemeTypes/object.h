#pragma once
#include "lcpp/core/schemeTypes/type.h"

namespace lcpp
{
    class LCPP_CORE_API SchemeObject
    {
    public:

        virtual ~SchemeObject() = 0 {}
        virtual bool operator==(const SchemeObject& other) const = 0;
        virtual ezString toString() const = 0;

        virtual SchemeType::Enum type() const { return SchemeType::Object; }
        virtual bool is(SchemeType::Enum type) const { return type == SchemeType::Object; }
    };

}
