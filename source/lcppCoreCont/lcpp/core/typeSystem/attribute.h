#pragma once

namespace lcpp
{
    class AttributeFlags
    {
    public:

        enum Bits
        {
            None = 0,

            Callable = EZ_BIT(0),
            Builtin  = EZ_BIT(1),
            Nameable = EZ_BIT(2),
            EnvironmentContainer = EZ_BIT(3),

            BITS_MAX = EZ_BIT(4),
        };

    public:

        AttributeFlags(ezUInt32 attributeFlags);

        ezUInt32 getFlags() const;

        bool isCallable() const;
        bool isBuiltin() const;
        bool isNameable() const;
        bool isEnvironmentContainer() const;

        void toString(ezStringBuilder& out_string) const;

    private:

        ezUInt32 m_flags;
    };

    bool operator ==(const AttributeFlags& rhs, const AttributeFlags& lhs);
    bool operator !=(const AttributeFlags& rhs, const AttributeFlags& lhs);

    const char* toString(AttributeFlags::Bits value);
}

#include "lcpp/core/typeSystem/impl/attribute.inl"
