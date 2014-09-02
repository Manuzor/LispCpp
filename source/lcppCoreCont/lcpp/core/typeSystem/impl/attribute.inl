
namespace lcpp
{
    EZ_FORCE_INLINE
    AttributeFlags::AttributeFlags(ezUInt32 attributeFlags) :
        m_flags(attributeFlags)
    {
    }

    EZ_FORCE_INLINE
    ezUInt32 AttributeFlags::getFlags() const
    {
        return m_flags;
    }

    EZ_FORCE_INLINE
    bool AttributeFlags::isCallable() const
    {
        return (m_flags & Callable) != 0;
    }

    EZ_FORCE_INLINE
    bool AttributeFlags::isBuiltin() const
    {
        return (m_flags & Builtin) != 0;
    }

    EZ_FORCE_INLINE
    bool AttributeFlags::isNameable() const
    {
        return (m_flags & Nameable) != 0;
    }

    EZ_FORCE_INLINE
    bool AttributeFlags::isEnvironmentContainer() const
    {
        return (m_flags & EnvironmentContainer) != 0;
    }
    
    EZ_FORCE_INLINE
    void AttributeFlags::toString(ezStringBuilder& out_string) const
    {
        if (m_flags == None)
        {
            out_string.Append("None");
            return;
        }

        for(auto currentBit = EZ_BIT(0); currentBit != BITS_MAX; currentBit <<= 1)
        {
            if ((m_flags & currentBit) != 0)
            {
                out_string.AppendFormat("%s | ", lcpp::toString((Bits)currentBit));
            }
        }

        out_string.Shrink(0, 3);
    }

    // Free functions.
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    bool operator==(const AttributeFlags& rhs, const AttributeFlags& lhs)
    {
        return rhs.getFlags() == lhs.getFlags();
    }

    EZ_FORCE_INLINE
    bool operator!=(const AttributeFlags& rhs, const AttributeFlags& lhs)
    {
        return rhs.getFlags() != lhs.getFlags();
    }

    EZ_FORCE_INLINE
    const char* toString(AttributeFlags::Bits value)
    {
        EZ_CHECK_AT_COMPILETIME_MSG(AttributeFlags::BITS_MAX == EZ_BIT(4),
                                    "AttributeFlags::Bits was changed. "
                                    "Please adjust the implementation of this method accordingly.");
        switch(value)
        {
        case AttributeFlags::None:                 return "None";
        case AttributeFlags::Callable:             return "Callable";
        case AttributeFlags::Builtin:              return "Builtin";
        case AttributeFlags::Nameable:             return "Nameable";
        case AttributeFlags::EnvironmentContainer: return "EnvironmentContainer";
        }

        return "<INVALID_INPUT>";
    }
}
