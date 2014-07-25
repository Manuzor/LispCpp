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
            Bool,

            Number,

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

        bool operator ==(const Type& rhs);
        bool operator !=(const Type& rhs);

    private:

        const Enum m_id;
    };
}

#include "lcpp/core/typeSystem/impl/type.inl"
