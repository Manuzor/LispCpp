#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class TypeInfo
    {
    public:

        const Type& getType() const;

        const String& getTypeName() const;

        const String& getPrettyName() const;

    private:

        Type m_type;
        String m_typeName;
        String m_prettyName;
    };
}

#include "lcpp/core/typeSystem/impl/typeInfo.inl"
