#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LCPP_CORE_API LispNil :
        public LispObject
    {
        friend class TypeFactory;
    public:

            /// \brief Returns the singleton instance of this class.
            ///
            /// Does not actually create a new instance every time but for the sake of consistency it is still named 'create'
        static Ptr<LispNil> create();

        static const Type& typeInfo();

    public:

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

    bool isNil(Ptr<LispObject> pObject);
    bool isNil(Ptr<const LispObject> pObject);
    bool isNil(Ptr<LispNil> pNil);
    bool isNil(Ptr<const LispNil> pNil);
}

#define LCPP_NIL (::lcpp::LispNil::create())

#include "lcpp/core/typeSystem/types/implementation/nil_inl.h"
