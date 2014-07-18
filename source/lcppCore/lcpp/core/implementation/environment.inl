
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

inline
ezString
lcpp::Environment::toString() const
{
    return qualifiedName();
}

inline
const lcpp::Type&
lcpp::Environment::type() const
{
    return Environment::typeInfo();
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::Environment::copy() const
{
    return Environment::createCopy(*this);
}

inline
bool
lcpp::operator==(const Environment& lhs, const Environment& rhs)
{
    return lhs.qualifiedName().IsEqual(rhs.qualifiedName().GetData());
}
