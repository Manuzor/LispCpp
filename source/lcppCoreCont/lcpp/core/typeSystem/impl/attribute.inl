
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
    const char* toString(AttributeFlags::Enum value)
    {
        switch(value)
        {
        case AttributeFlags::None: return "None";
        case AttributeFlags::Callable: return "Callable";
        }

        return "<INVALID_INPUT>";
    }
}
