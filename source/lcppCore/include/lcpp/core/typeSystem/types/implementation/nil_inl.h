// Static
inline
lcpp::SchemeNil&
lcpp::SchemeNil::instance()
{
    static SchemeNil nil;
    return nil;
}


inline
lcpp::SchemeNil::SchemeNil() :
    m_pNil(nullptr)
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
    return obj.is(TypeInfo<SchemeNil>::type());
}
inline
ezString
lcpp::SchemeNil::toString() const
{
    static ezString voidString("()");
    return voidString;
}

//////////////////////////////////////////////////////////////////////////

inline
bool
lcpp::isNil(const lcpp::SchemeObject* pObject)
{
    return pObject == &SCHEME_NIL;
}

inline
bool
lcpp::isNil(const lcpp::SchemeObject& object)
{
    return isNil(&object);
}
