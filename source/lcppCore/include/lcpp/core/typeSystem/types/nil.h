#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeNil :
        public SchemeExtend<SchemeNil, SchemeObject>
    {
    public:

        static const SchemeNil& instance();

        SchemeNil();
        virtual ~SchemeNil();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeNil);
}
#define SCHEME_NIL (::lcpp::SchemeNil::instance())

#include "lcpp/core/typeSystem/types/implementation/nil_inl.h"
