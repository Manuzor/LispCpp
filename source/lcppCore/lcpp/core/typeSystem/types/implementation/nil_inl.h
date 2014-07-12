
inline
lcpp::LispNil::LispNil() :
    m_pNil(42u)
{
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispNil::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NIL;
}

inline
const lcpp::Type&
lcpp::LispNil::type() const
{
    return LispNil::typeInfo();
}

inline
bool
lcpp::LispNil::operator ==(const LispObject& obj) const
{
    return obj.is<LispNil>();
}

inline
ezString
lcpp::LispNil::toString() const
{
    static ezString voidString("()");
    return voidString;
}

//////////////////////////////////////////////////////////////////////////

inline
bool
lcpp::isNil(Ptr<LispObject> pObject)
{
    return pObject == LCPP_NIL;
}

inline
bool
lcpp::isNil(Ptr<const LispObject> pObject)
{
    return pObject == LCPP_NIL;
}

inline
bool
lcpp::isNil(Ptr<LispNil> pNil)
{
    EZ_ASSERT(pNil == LCPP_NIL, "LispNil is supposed to be a singleton!");
    return true;
}

inline
bool
lcpp::isNil(Ptr<const LispNil> pNil)
{
    EZ_ASSERT(pNil == LCPP_NIL, "LispNil is supposed to be a singleton!");
    return true;
}
