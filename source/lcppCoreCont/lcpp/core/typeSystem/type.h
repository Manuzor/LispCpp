#pragma once

namespace lcpp
{
    class Type
    {
    public:

        enum Enum
        {
            ENUM_MIN = -1,

            Nil,
            Void,
            True,
            False,

            Integer,
            Float,

            Symbol,
            String,

            Cons,
            Lambda,
            Syntax,

            File,

            ENUM_COUNT
        };

    public:

        Type(Enum id);

        Enum getId() const;

        const char* toString() const;

    private:

        Enum m_id;
    };

    bool operator ==(const Type& rhs, const Type& lhs);
    bool operator !=(const Type& rhs, const Type& lhs);

    const char* toString(Type::Enum value);
}

#include "lcpp/core/typeSystem/impl/type.inl"
