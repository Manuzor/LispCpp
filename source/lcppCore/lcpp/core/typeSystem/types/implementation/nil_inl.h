﻿// Static
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
    return obj.is<SchemeNil>();
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
lcpp::isNil(const SchemeObject& object)
{
    return SCHEME_NIL == object;
}

inline
bool
lcpp::isNil(const SchemeObject* pObject)
{
    EZ_ASSERT(pObject != nullptr, "No SchemeObject* should be a nullptr, ever!");
    return isNil(*pObject);
}

inline
bool
lcpp::isNil(Ptr<SchemeObject> pObject)
{
    return isNil(pObject.get());
}

inline
bool
lcpp::isNil(Ptr<const SchemeObject> pObject)
{
    return isNil(pObject.get());
}