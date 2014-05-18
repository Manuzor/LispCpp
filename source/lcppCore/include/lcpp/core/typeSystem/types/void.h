#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeVoid :
        public SchemeExtend<SchemeVoid, SchemeObject>
    {
    public:

        static SchemeVoid& instance();

        SchemeVoid();
        virtual ~SchemeVoid();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeVoid);
}

#define SCHEME_VOID (::lcpp::SchemeVoid::instance())

#include "lcpp/core/typeSystem/types/implementation/void_inl.h"
