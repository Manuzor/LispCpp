
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
lcpp::SchemeBool::operator ==( const SchemeObject& obj ) const
{
    if (obj.is(SchemeType::Bool))
    {
        return static_cast<const SchemeBool&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::SchemeBool::operator ==( const SchemeBool& rhs ) const
{
    return rhs.m_value == m_value;
}

inline
ezString
lcpp::SchemeBool::toString() const
{
    return m_value ? "#t" : "#f";
}

inline
lcpp::SchemeBool::operator bool() const
{
    return m_value;
}
