﻿inline
lcpp::Type::Type() :
    id(makeUniqueId()),
    name(nullptr),
    memory()
{
}

inline
ezUInt64
lcpp::Type::makeUniqueId()
{
    // TODO Find a more elegant solution?
    static ezUInt64 counter = 0;
    return counter++;
}

inline
bool
lcpp::operator ==(const Type& lhs, const Type& rhs)
{
    return lhs.id == rhs.id;
}