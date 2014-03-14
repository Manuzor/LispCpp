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

    /// \brief Class that provides information about scheme types statically.
    /// \remark You have to specialize this template for all your scheme types!
    template<typename T>
    struct SchemeTypeInfo
    {
        /// \brief 
        static SchemeType::Enum type();

        /// \brief Number of bytes an instance of T allocates. Typically sizeof(T).
        static size_t size();

        /// \brief A human-readable name of this type.
        static const char* name();
    };

// Assumes that the type is called Scheme<typeValue>, e.g. for typeValue = Bool => SchemeBool
#define LCPP_SCHEME_TYPE_DECLARATION(typeValue) virtual ::lcpp::SchemeType::Enum type() const override { return ::lcpp::SchemeType::typeValue; }\
    virtual bool is(::lcpp::SchemeType::Enum type) const override { return type == ::lcpp::SchemeType::typeValue; }
}
