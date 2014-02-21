#pragma once
#include "lcpp/core/schemeTypes/object.h"

namespace lcpp
{
    class LCPP_CORE_API SchemeVoid :
        public SchemeObject
    {
    public:
        LCPP_SCHEME_TYPE_DECLARATION(Void);

        SchemeVoid();
        virtual ~SchemeVoid();

        virtual const SchemeBool& operator==(const SchemeObject& obj) const override;
        virtual ezString toString() const override;

    private:
        EZ_DISALLOW_COPY_AND_ASSIGN(SchemeVoid);
    };
}
