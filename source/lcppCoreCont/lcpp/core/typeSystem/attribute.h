#pragma once

namespace lcpp
{
    class AttributeFlags
    {
    public:

        enum Enum
        {
            None = 0,

            Callable = EZ_BIT(0),
        };

    public:

        AttributeFlags(ezUInt32 attributeFlags);

        ezUInt32 getFlags() const;

        bool isCallable() const;

    private:

        ezUInt32 m_flags;
    };

    bool operator ==(const AttributeFlags& rhs, const AttributeFlags& lhs);
    bool operator !=(const AttributeFlags& rhs, const AttributeFlags& lhs);

    const char* toString(AttributeFlags::Enum value);
}

#include "lcpp/core/typeSystem/impl/attribute.inl"
