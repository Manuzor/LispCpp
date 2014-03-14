#pragma once
#include "lcpp/core/schemeTypes/object.h"

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
        static size_t size() { return sizeof(SchemeNil); }
        static SchemeType::Enum type() { return SchemeType::Nil; }
        static const char* name() { return "SchemeNil"; }
    };

#define SCHEME_NIL (::lcpp::SchemeNil::instance())
}
