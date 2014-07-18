
inline
const lcpp::Type&
lcpp::LispFunction::type() const
{
    return LispFunction::typeInfo();
}

inline
bool
lcpp::operator==(const LispFunction& lhs, const LispFunction& rhs)
{
    // Identity check.
    return &lhs == &rhs;
}

inline
lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispFunction::env()
{
    return m_pEnv;
}

inline
lcpp::Ptr<const lcpp::LispEnvironment>
lcpp::LispFunction::env() const
{
    return m_pEnv;
}
