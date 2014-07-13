
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
