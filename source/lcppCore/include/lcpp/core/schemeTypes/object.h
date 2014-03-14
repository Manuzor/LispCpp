#pragma once
#include "lcpp/core/schemeTypes/type.h"

namespace lcpp
{
    template<typename Derived, typename Base>
    class SchemeExtend;

    /// \brief Base class for all scheme types
    /// 
    /// Do not directly derive from your scheme type from this.
    /// Use SchemeExtend instead.
    /// Example:
    /// class SchemeBool : public SchemeExtend<SchemeBool, SchemeObject> { /* ... */ };
    class SchemeObject
    {
    public:

        virtual ~SchemeObject() = 0 {}
        virtual bool operator ==(const SchemeObject& other) const = 0;
        virtual bool operator !=(const SchemeObject& rhs) const { return !(*this == rhs); }
        virtual ezString toString() const = 0;

        virtual SchemeType::Enum type() const = 0;
        virtual bool is(SchemeType::Enum type) const = 0;

        virtual size_t size() const = 0;
        virtual size_t alignment() const = 0;
    };

    template<>
    struct SchemeTypeInfo<SchemeObject>
    {
        static size_t size() { return sizeof(SchemeObject); }
        static SchemeType::Enum type() { return SchemeType::Object; }
        static const char* name() { return "SchemeObject"; }
    };

    /// \brief ...
    ///
    /// Has to derive from SchemeObject at some point!
    template<typename Derived, typename Base>
    class SchemeExtend : public Base
    {
    public:
        virtual ~SchemeExtend() = 0 {}

        virtual SchemeType::Enum type() const override { return SchemeTypeInfo<Derived>::type(); }
        virtual bool is(SchemeType::Enum type) const override { return type == SchemeTypeInfo<Derived>::type(); }

        virtual size_t size() const override { return sizeof(Derived); }
        virtual size_t alignment() const override { return EZ_ALIGNMENT_OF(Derived); }
    };
}
