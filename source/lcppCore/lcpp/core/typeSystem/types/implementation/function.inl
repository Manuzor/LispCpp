
inline
const lcpp::Type&
lcpp::LispFunction::type() const
{
    return LispFunction::typeInfo();
}

inline
ezString&
lcpp::LispFunction::name()
{
    return m_name;
}

inline
const ezString&
lcpp::LispFunction::name() const
{
    return m_name;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::LispFunction::env()
{
    return m_pEnv;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::LispFunction::env() const
{
    return m_pEnv;
}
