inline
lcpp::SchemeTypeId::SchemeTypeId() :
    id(makeUniqueId()),
    name(nullptr),
    size(0),
    alignment(0)
{
}

inline
ezUInt64
lcpp::SchemeTypeId::makeUniqueId()
{
    // TODO Find a more elegant solution?
    static ezUInt64 counter = 0;
    return counter++;
}

inline
bool
lcpp::operator ==(const SchemeTypeId& lhs, const SchemeTypeId& rhs)
{
    return lhs.id == rhs.id;
}