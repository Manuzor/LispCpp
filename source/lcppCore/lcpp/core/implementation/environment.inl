
inline
lcpp::Ptr<lcpp::Environment>
lcpp::Environment::parent()
{
    return m_pParent;
}

inline
lcpp::Ptr<const lcpp::Environment>
lcpp::Environment::parent() const
{
    return m_pParent;
}

inline
const ezString&
lcpp::Environment::name() const
{
    return m_name;
}

inline
ezString&
lcpp::Environment::name()
{
    return m_name;
}

inline
ezString
lcpp::Environment::qualifiedName() const
{
    if(!m_pParent) { return "/"; }

    ezStringBuilder builder;
    qualifiedNameHelper(builder);
    return builder;
}

inline
void
lcpp::Environment::qualifiedNameHelper(ezStringBuilder& builder) const
{
    if (!m_pParent) { return; }

    m_pParent->qualifiedNameHelper(builder);
    builder.Append('/');
    builder.Append(m_name.GetData());
}


