#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{

    /// \brief This class is meant to have a unique, immutable instance for every needed type in the system.
    class TypeInfo
    {
    public:

        TypeInfo(const Type& type);

        TypeInfo(const Type& type, const String& prettyName);

        const Type& getType() const;

        const String& getPrettyName() const;

    private:

        Type m_type;

        String m_prettyName;

    };
}

#include "lcpp/core/typeSystem/impl/typeInfo.inl"
