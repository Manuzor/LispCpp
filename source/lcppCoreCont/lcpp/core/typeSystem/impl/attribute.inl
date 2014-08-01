
namespace lcpp
{
    EZ_FORCE_INLINE
    Attribute::Attribute(ezUInt32 attributeFlags) :
        m_flags(attributeFlags)
    {
    }

    EZ_FORCE_INLINE
    ezUInt32 Attribute::getFlags() const
    {
        return m_flags;
    }

    EZ_FORCE_INLINE
    bool Attribute::isCallable() const
    {
        return (m_flags & Callable) != 0;
    }

    // Free functions.
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    bool operator==(const Attribute& rhs, const Attribute& lhs)
    {
        return rhs.getFlags() == lhs.getFlags();
    }

    EZ_FORCE_INLINE
    bool operator!=(const Attribute& rhs, const Attribute& lhs)
    {
        return rhs.getFlags() != lhs.getFlags();
    }

    EZ_FORCE_INLINE
    const char* toString(Attribute::Enum value)
    {
        switch(value)
        {
        case Attribute::None: return "None";
        case Attribute::Callable: return "Callable";
        }

        return "<INVALID_INPUT>";
    }
}
