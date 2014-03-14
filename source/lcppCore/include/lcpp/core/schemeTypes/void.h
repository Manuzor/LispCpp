#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class SchemeVoid :
        public SchemeExtend<SchemeVoid, SchemeObject>
    {
    public:

        static const SchemeVoid& instance();

        SchemeVoid();
        virtual ~SchemeVoid();

        virtual bool operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;
    };

    template<>
    struct SchemeTypeInfo<SchemeVoid>
    {
        static size_t size() { return sizeof(SchemeVoid); }
        static SchemeType::Enum type() { return SchemeType::Void; }
        static const char* name() { return "SchemeVoid"; }
    };

#define SCHEME_VOID (::lcpp::SchemeVoid::instance())
}
