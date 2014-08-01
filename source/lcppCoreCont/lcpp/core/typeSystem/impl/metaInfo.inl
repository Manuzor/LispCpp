
namespace lcpp
{
    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type) :
        m_type(type),
        m_attributes(Attribute::None),
        m_prettyName(m_type.toString())
    {
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type,
                       const String& prettyName) :
        m_type(type),
        m_attributes(Attribute::None),
        m_prettyName(prettyName)
    {
    }

    EZ_FORCE_INLINE
    MetaInfo::MetaInfo(const Type& type,
                       const Attribute& attributes,
                       const String& prettyName) :
        m_type(type),
        m_attributes(attributes),
        m_prettyName(prettyName)
    {
    }

    EZ_FORCE_INLINE
    const Type& MetaInfo::getType() const
    {
        return m_type;
    }

    EZ_FORCE_INLINE
    const Attribute& MetaInfo::getAttributes() const
    {
        return m_attributes;
    }

    EZ_FORCE_INLINE
    const String& MetaInfo::getPrettyName() const
    {
        return m_prettyName;
    }
}
