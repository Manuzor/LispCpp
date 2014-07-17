
inline
lcpp::LispBool::LispBool(bool value) :
    m_value(value)
{
}

inline
const lcpp::Type&
lcpp::LispBool::type() const
{
    return LispBool::typeInfo();
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispBool::copy() const
{
    return create(m_value);
}

inline
bool
lcpp::LispBool::operator ==( const LispObject& rhs ) const
{
    if(rhs.is<LispBool>())
    {
        return *this == static_cast<const LispBool&>(rhs);
    }
    return false;
}

inline
ezString
lcpp::LispBool::toString() const
{
    static auto trueString = ezString("#t");
    static auto falseString = ezString("#f");

    return m_value ? trueString : falseString;
}

inline
bool
lcpp::LispBool::value() const
{
    return m_value;
}

inline
bool
lcpp::isTrue(Ptr<LispObject> pObject)
{
    return pObject == LCPP_TRUE;
}

inline
bool
lcpp::isFalse(Ptr<LispObject> pObject)
{
    return pObject == LCPP_FALSE;
}

inline
bool
lcpp::operator ==(const LispBool& lhs, const LispBool& rhs)
{
    return lhs.value() == rhs.value();
}
