#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class LCPP_CORE_API SchemeBool :
        public SchemeObject
    {
    public:
        LCPP_SCHEME_TYPE_DECLARATION(Bool);

        SchemeBool();
        virtual ~SchemeBool();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

        operator bool() const;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeBool);
    };
}
