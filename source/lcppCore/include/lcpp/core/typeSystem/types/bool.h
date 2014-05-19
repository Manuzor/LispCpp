#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeBool :
        public SchemeExtend<SchemeBool, SchemeObject>
    {
        friend class TypeFactory;
    public:

        static SchemeBool& trueInstance();
        static SchemeBool& falseInstance();

        virtual ~SchemeBool();

        virtual bool operator==(const SchemeObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        bool value() const;

    private:
        const bool m_value;

        SchemeBool(bool value);
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeBool);

    bool operator ==(const SchemeBool& lhs, const SchemeBool& rhs);

#define SCHEME_TRUE (::lcpp::SchemeBool::trueInstance())
#define SCHEME_FALSE (::lcpp::SchemeBool::falseInstance())

}

#include "lcpp/core/typeSystem/types/implementation/bool_inl.h"
