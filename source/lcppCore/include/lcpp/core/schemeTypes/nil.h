#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class LCPP_CORE_API SchemeNil :
        public SchemeObject
    {
    public:
        SCHEME_TYPE_DECLARATION(Nil);

        SchemeNil();
        virtual ~SchemeNil();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeNil);
    };
}
