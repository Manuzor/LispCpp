
inline
const lcpp::Type&
lcpp::SchemeFunction::type() const
{
    return TypeInfo<SchemeFunction>::type();
}

inline
ezString&
lcpp::SchemeFunction::name()
{
    return m_name;
}

inline
const ezString&
lcpp::SchemeFunction::name() const
{
    return m_name;
}

inline
lcpp::Ptr<lcpp::Environment>
lcpp::SchemeFunction::env()
{
    return m_pEnv;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::SchemeFunction::env() const
{
    return m_pEnv;
}