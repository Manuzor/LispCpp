// Static
inline
const lcpp::SchemeNil&
lcpp::SchemeNil::instance()
{
    static SchemeNil nil;
    return nil;
}


inline
lcpp::SchemeNil::SchemeNil()
{
}

inline
lcpp::SchemeNil::~SchemeNil()
{
}

inline
bool
lcpp::SchemeNil::operator ==(const SchemeObject& obj) const
{
    return obj.is(SchemeTypeInfo<SchemeNil>::type());
}
inline
ezString
lcpp::SchemeNil::toString() const
{
    static ezString voidString("()");
    return voidString;
}
