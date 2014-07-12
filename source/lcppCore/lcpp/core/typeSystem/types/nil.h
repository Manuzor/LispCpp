#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispNil :
        public LispObject
    {
        friend class TypeFactory;
    public:

        LCPP_CORE_API static Ptr<LispNil> instance();
        LCPP_CORE_API static const Type& typeInfo();

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

    private:
        // For debugging. Indicates that this instance is the real thing.
        const ezUInt8 m_pNil;

        LispNil();
        EZ_DISALLOW_COPY_AND_ASSIGN(LispNil);
    };

    template<>
    struct TypeInfo <LispNil>
    {
        static const Type& type() { return LispNil::typeInfo(); }
    };

    bool isNil(Ptr<LispObject> pObject);
    bool isNil(Ptr<const LispObject> pObject);
    bool isNil(Ptr<LispNil> pNil);
    bool isNil(Ptr<const LispNil> pNil);
}

#define LCPP_NIL (::lcpp::LispNil::instance())

#include "lcpp/core/typeSystem/types/implementation/nil_inl.h"
