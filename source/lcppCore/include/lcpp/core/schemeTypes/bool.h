#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class SchemeBool :
        public SchemeExtend<SchemeBool, SchemeObject>
    {
    public:

        static const SchemeBool& trueInstance();
        static const SchemeBool& falseInstance();

        SchemeBool(bool value);
        virtual ~SchemeBool();

        virtual bool operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        bool operator ==(const SchemeBool& rhs) const;

        operator bool() const;

    private:
        const bool m_value;
    };

    template<>
    struct SchemeTypeInfo<SchemeBool>
    {
        static size_t size() { return sizeof(SchemeBool); }
        static SchemeType::Enum type() { return SchemeType::Bool; }
        static const char* name() { return "SchemeBool"; }
    };

#define SCHEME_TRUE (::lcpp::SchemeBool::trueInstance())
#define SCHEME_FALSE (::lcpp::SchemeBool::falseInstance())

}
