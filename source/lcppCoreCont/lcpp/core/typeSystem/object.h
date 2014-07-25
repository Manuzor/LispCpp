#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeInfo.h"

namespace lcpp
{
    class LCPP_API_CORE_CONT LispObject
    {
    public:

        bool isType(const Type& type) const;
        const Type& getType() const;

    private:

        Ptr<TypeInfo> m_pTypeInfo;

    private:

        LispObject();

    };
}

#include "lcpp/core/typeSystem/impl/object.inl"
