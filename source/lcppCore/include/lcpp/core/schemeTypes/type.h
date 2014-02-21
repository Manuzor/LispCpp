#pragma once

namespace lcpp
{
    struct SchemeType
    {
        enum Enum
        {
            INVALID = -1,

            Object,
            Void,
            Nil,
            Bool,
            Symbol,
            Cons,
            Number,
            String,

            NUM_ELEMENTS
        };

        LCPP_DISALLOW_CONSTRUCTION(SchemeType);
    };

#define LCPP_SCHEME_TYPE_DECLARATION(typeValue) virtual ::lcpp::SchemeType::Enum type() const override { return ::lcpp::SchemeType::typeValue; }\
    virtual const ::lcpp::SchemeBool& is(::lcpp::SchemeType::Enum type) const override { return convert(type == ::lcpp::SchemeType::typeValue); }

}
