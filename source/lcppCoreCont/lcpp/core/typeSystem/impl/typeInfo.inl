
namespace lcpp
{
    TypeInfo::TypeInfo(const Type& type) :
        m_type(type),
        m_prettyName(m_type.toString())
    {
    }

    TypeInfo::TypeInfo(const Type& type, const String& prettyName) :
        m_type(type),
        m_prettyName(prettyName)
    {
    }

    const Type& TypeInfo::getType() const
    {
        return m_type;
    }

    const String& TypeInfo::getPrettyName() const
    {
        return m_prettyName;
    }
}
