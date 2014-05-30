#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeVoid :
        public SchemeExtend<SchemeVoid, SchemeObject>
    {
        friend class TypeFactory;
    public:

        static SchemeVoid& instance();

        virtual ~SchemeVoid();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;
    private:

        SchemeVoid();
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeVoid);
}

#define SCHEME_VOID (::lcpp::SchemeVoid::instance())
#define SCHEME_VOID_PTR (&SCHEME_VOID)

#include "lcpp/core/typeSystem/types/implementation/void_inl.h"
