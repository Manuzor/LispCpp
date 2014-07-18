
inline
lcpp::Type::Type(ezUInt32 id,
                 ezUInt32 flags,
                 const char* typeName,
                 const char* name,
                 const MemoryInfo& memInfo) :
    id(id),
    flags(flags),
    typeName(typeName),
    name(name),
    memory(memInfo)
{
}

inline
bool
lcpp::Type::isAbstract() const
{
    return (flags & Flags::Abstract) != 0;
}

inline
bool
lcpp::Type::isUserDefined() const
{
    return (flags & Flags::UserDefined) != 0;
}

inline
bool
lcpp::Type::isSyntax() const
{
    return (flags & Flags::Syntax) != 0;
}

inline
bool
lcpp::Type::isCallable() const
{
    return (flags & Flags::Callable) != 0;
}

inline
bool
lcpp::Type::isArithmetic() const
{
    return (flags & Flags::Arithmetic) != 0;
}

inline
bool
lcpp::Type::isSingleton() const
{
    return (flags & Flags::Singleton) != 0;
}

inline
bool
lcpp::Type::isNameable() const
{
    return (flags & Flags::Nameable) != 0;
}

inline
bool
lcpp::operator ==(const Type& lhs, const Type& rhs)
{
    return lhs.id == rhs.id;
}

inline
bool
lcpp::operator !=(const Type& lhs, const Type& rhs)
{
    return !(lhs == rhs);
}
