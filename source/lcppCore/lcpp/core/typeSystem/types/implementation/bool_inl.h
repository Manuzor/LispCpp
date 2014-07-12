// static
inline
lcpp::LispBool&
lcpp::LispBool::trueInstance()
{
    static LispBool t(true);
    return t;
}

// static
inline
lcpp::LispBool&
lcpp::LispBool::falseInstance()
{
    static LispBool f(false);
    return f;
}


inline
lcpp::LispBool::LispBool(bool value) :
    m_value(value)
{
}

inline
lcpp::LispBool::~LispBool()
{
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
    static ezString trueString("#t");
    static ezString falseString("#f");

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
    return pObject == SCHEME_TRUE_PTR;
}

inline
bool
lcpp::isFalse(Ptr<LispObject> pObject)
{
    return pObject == SCHEME_FALSE_PTR;
}

inline
bool
lcpp::operator ==(const LispBool& lhs, const LispBool& rhs)
{
    return lhs.value() == rhs.value();
}
