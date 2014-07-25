
namespace lcpp
{
    const Type& TypeInfo::getType() const
    {
        return m_type;
    }

    const String& TypeInfo::getTypeName() const
    {
        return m_typeName;
    }

    const String& TypeInfo::getPrettyName() const
    {
        return m_prettyName;
    }
}
