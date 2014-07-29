
namespace lcpp
{
    inline
    Type::Type(Enum id) :
        m_id(id)
    {
    }

    inline
    Type::Enum Type::getId() const
    {
        return m_id;
    }

    inline
    const char* Type::toString() const
    {
        return lcpp::toString(m_id);
    }

    // Free functions.
    //////////////////////////////////////////////////////////////////////////

    inline
    bool operator==(const Type& rhs, const Type& lhs)
    {
        return rhs.getId() == lhs.getId();
    }

    inline
    bool operator!=(const Type& rhs, const Type& lhs)
    {
        return rhs.getId() != lhs.getId();
    }

    inline
    const char* toString(Type::Enum value)
    {
        static_assert(Type::ENUM_COUNT == 14,
                      "Did you add something new to Type::Enum and forgot to update this function?");

        switch(value)
        {
        case Type::Nil:  return "Nil";
        case Type::Void: return "Void";
        case Type::True: return "True";
        case Type::False: return "False";

        case Type::Integer: return "Integer";
        case Type::Float: return "Float";

        case Type::Symbol: return "Symbol";
        case Type::String: return "String";

        case Type::Cons:        return "Cons";
        case Type::Lambda:      return "Lambda";
        case Type::Syntax:      return "Syntax";
        case Type::Environment: return "Environment";

        case Type::File: return "File";

        case Type::Continuation: return "Continuation";
        }

        return "<INVALID_INPUT>";
    }
}
