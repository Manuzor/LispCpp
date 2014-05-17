
inline
lcpp::Type::Type(ezUInt32 id, const char* name, MemoryInfo memInfo) :
    id(id),
    name(name),
    memory(memInfo)
{
    printf("+++ Type for %s with id %u\n", name, id);
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
    return lhs.id != rhs.id;
}
