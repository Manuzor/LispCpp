
namespace lcpp
{
    inline
    MetaInfo::MetaInfo(const Type& type) :
        m_type(type),
        m_prettyName(m_type.toString())
    {
    }

    inline
    MetaInfo::MetaInfo(const Type& type, const String& prettyName) :
        m_type(type),
        m_prettyName(prettyName)
    {
    }

    inline
    const Type& MetaInfo::getType() const
    {
        return m_type;
    }

    inline
    const String& MetaInfo::getPrettyName() const
    {
        return m_prettyName;
    }
}
