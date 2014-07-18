
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
void
lcpp::Environment::name(Ptr<LispSymbol> pName)
{
    m_pName = pName;
}

inline
lcpp::Ptr<const lcpp::LispSymbol>
lcpp::Environment::name() const
{
    return m_pName;
}

inline
lcpp::Ptr<lcpp::LispSymbol>
lcpp::Environment::name()
{
    return m_pName;
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
