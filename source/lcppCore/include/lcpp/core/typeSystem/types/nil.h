#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeNil :
        public SchemeExtend<SchemeNil, SchemeObject>
    {
        friend class TypeFactory;
    public:

        static SchemeNil& instance();
        virtual ~SchemeNil();

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

    private:
        // For debugging. Shows that this instance is the real nil
        const void* const m_pNil;

        SchemeNil();
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeNil);

    bool isNil(const SchemeObject* pObject);
    bool isNil(const SchemeObject& object);
}
#define SCHEME_NIL (::lcpp::SchemeNil::instance())

#include "lcpp/core/typeSystem/types/implementation/nil_inl.h"
