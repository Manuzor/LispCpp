#pragma once
#include "lcpp/core/schemeTypeSystem/object.h"

namespace lcpp
{
    class SchemeNil :
        public SchemeExtend<SchemeNil, SchemeObject>
    {
    public:

        static const SchemeNil& instance();

        SchemeNil();
        virtual ~SchemeNil();

        virtual bool operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;
    };

    template<>
    struct SchemeTypeInfo<SchemeNil>
    {
        inline static size_t size() { return sizeof(SchemeNil); }
        inline static SchemeType::Enum type() { return SchemeType::Nil; }
        inline static const char* name() { return "SchemeNil"; }
    };

#define SCHEME_NIL (::lcpp::SchemeNil::instance())
}

#include "lcpp/core/schemeTypeSystem/types/implementation/nil_inl.h"
