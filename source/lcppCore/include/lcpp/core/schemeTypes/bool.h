#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class LCPP_CORE_API SchemeBool :
        public SchemeObject
    {
    public:
        LCPP_SCHEME_TYPE_DECLARATION(Bool);

        SchemeBool(bool value);
        virtual ~SchemeBool();

        virtual bool operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        bool operator ==(const SchemeBool& rhs) const;

        operator bool() const;

    private:
        const bool m_value;
    };

#define SCHEME_TRUE ::lcpp::SchemeBool(true)
#define SCHEME_FALSE ::lcpp::SchemeBool(false)

}
