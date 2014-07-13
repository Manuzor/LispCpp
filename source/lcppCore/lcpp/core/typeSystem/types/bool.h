﻿#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LCPP_CORE_API LispBool :
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispBool> trueInstance();
        static Ptr<LispBool> falseInstance();

        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator==(const LispObject& rhs) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;

        bool value() const;

    private:
        const bool m_value;

        LispBool(bool value);

    };

    bool isTrue(Ptr<LispObject> pObject);
    bool isFalse(Ptr<LispObject> pObject);

    bool operator ==(const LispBool& lhs, const LispBool& rhs);

}

#define LCPP_TRUE  (::lcpp::LispBool::trueInstance())
#define LCPP_FALSE (::lcpp::LispBool::falseInstance())

#include "lcpp/core/typeSystem/types/implementation/bool_inl.h"
