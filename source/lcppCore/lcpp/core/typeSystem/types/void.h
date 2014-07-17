#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LCPP_CORE_API LispVoid :
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispVoid> create();
        static const Type& typeInfo();

    public:

        virtual ~LispVoid();

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;

    private:

        LispVoid();
    };
}

#define LCPP_VOID (lcpp::LispVoid::create())

#include "lcpp/core/typeSystem/types/implementation/void.inl"
