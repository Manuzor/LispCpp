// static
inline
lcpp::SchemeBool&
lcpp::SchemeBool::trueInstance()
{
    static SchemeBool t(true);
    return t;
}

// static
inline
lcpp::SchemeBool&
lcpp::SchemeBool::falseInstance()
{
    static SchemeBool f(false);
    return f;
}


inline
lcpp::SchemeBool::SchemeBool(bool value) :
    m_value(value)
{
}

inline
lcpp::SchemeBool::~SchemeBool()
{
}

inline
bool
lcpp::SchemeBool::operator ==( const SchemeObject& rhs ) const
{
    if (rhs.is(TypeInfo<SchemeBool>::type()))
    {
        return *this == static_cast<const SchemeBool&>(rhs);
    }
    return false;
}

inline
ezString
lcpp::SchemeBool::toString() const
{
    static ezString trueString("#t");
    static ezString falseString("#f");

    return m_value ? trueString : falseString;
}

inline
lcpp::SchemeBool::operator bool() const
{
    return m_value;
}

inline
bool
lcpp::SchemeBool::value() const
{
    return m_value;
}

inline
bool
lcpp::operator ==(const SchemeBool& lhs, const SchemeBool& rhs)
{
    return lhs.value() == rhs.value();
}
