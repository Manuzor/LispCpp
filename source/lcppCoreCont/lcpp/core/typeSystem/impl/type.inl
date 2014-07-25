
namespace lcpp
{
    Type::Type(Enum id) :
        m_id(id)
    {
    }

    bool Type::operator==(const Type& rhs)
    {
        return m_id == rhs.m_id;
    }

    bool Type::operator!=(const Type& rhs)
    {
        return m_id != rhs.m_id;
    }

    Type::Enum Type::getId() const
    {
        return m_id;
    }

}
