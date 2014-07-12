// Static
inline
lcpp::LispNil&
lcpp::LispNil::instance()
{
    static LispNil nil;
    return nil;
}


inline
lcpp::LispNil::LispNil() :
    m_pNil(nullptr)
{
}

inline
lcpp::LispNil::~LispNil()
{
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
lcpp::isNil(const LispObject& object)
{
    return SCHEME_NIL == object;
}

inline
bool
lcpp::isNil(const LispObject* pObject)
{
    EZ_ASSERT(pObject != nullptr, "No LispObject* should be a nullptr, ever!");
    return isNil(*pObject);
}

inline
bool
lcpp::isNil(Ptr<LispObject> pObject)
{
    return isNil(pObject.get());
}

inline
bool
lcpp::isNil(Ptr<const LispObject> pObject)
{
    return isNil(pObject.get());
}
