
namespace lcpp
{
    inline
    TypeInfo::TypeInfo(const Type& type) :
        m_type(type),
        m_prettyName(m_type.toString())
    {
    }

    inline
    TypeInfo::TypeInfo(const Type& type, const String& prettyName) :
        m_type(type),
        m_prettyName(prettyName)
    {
    }

    inline
    const Type& TypeInfo::getType() const
    {
        return m_type;
    }

    inline
    const String& TypeInfo::getPrettyName() const
    {
        return m_prettyName;
    }
}
