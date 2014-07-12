#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispNil :
        public LispExtend<LispNil, LispObject>
    {
        friend class TypeFactory;
    public:

        static LispNil& instance();
        virtual ~LispNil();

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

    private:
        // For debugging. Shows that this instance is the real nil
        const void* const m_pNil;

        LispNil();
    };

    DECLARE_SCHEME_TYPE_INFO(LispNil);

    bool isNil(const LispObject& object);
    bool isNil(const LispObject* pObject);
    bool isNil(Ptr<LispObject> pObject);
    bool isNil(Ptr<const LispObject> pObject);
}
#define SCHEME_NIL (::lcpp::LispNil::instance())
#define SCHEME_NIL_PTR (&SCHEME_NIL)

#include "lcpp/core/typeSystem/types/implementation/nil_inl.h"
