
inline
lcpp::Type::Type(ezUInt32 id, const char* typeName, const char* name, MemoryInfo memInfo) :
    id(id),
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
