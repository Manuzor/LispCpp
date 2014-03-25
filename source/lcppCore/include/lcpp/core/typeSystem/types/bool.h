#pragma once
#include "lcpp/core/typeSystem/object.h"

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

        virtual bool operator==(const SchemeObject& rhs) const override;
        virtual ezString toString() const override;

        operator bool() const;

        bool value() const;

    private:
        const bool m_value;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeBool);

    bool operator ==(const SchemeBool& lhs, const SchemeBool& rhs);

#define SCHEME_TRUE (::lcpp::SchemeBool::trueInstance())
#define SCHEME_FALSE (::lcpp::SchemeBool::falseInstance())

}

#include "lcpp/core/typeSystem/types/implementation/bool_inl.h"
