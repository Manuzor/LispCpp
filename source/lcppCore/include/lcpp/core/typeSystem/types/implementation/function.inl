
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
    return &m_env;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::SchemeFunction::env() const
{
    return &m_env;
}
