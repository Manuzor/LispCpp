#pragma once

namespace lcpp
{
    class Attribute
    {
    public:

        enum Enum
        {
            None = 0,

            Callable = EZ_BIT(0),
        };

    public:

        Attribute(ezUInt32 attributeFlags);

        ezUInt32 getFlags() const;

        bool isCallable() const;

    private:

        ezUInt32 m_flags;
    };

    bool operator ==(const Attribute& rhs, const Attribute& lhs);
    bool operator !=(const Attribute& rhs, const Attribute& lhs);

    const char* toString(Attribute::Enum value);
}

#include "lcpp/core/typeSystem/impl/attribute.inl"
