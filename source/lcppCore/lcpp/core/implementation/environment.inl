
inline
lcpp::Ptr<lcpp::LispEnvironment>
lcpp::LispEnvironment::parent()
{
    return m_pParent;
}

inline
lcpp::Ptr<const lcpp::LispEnvironment>
lcpp::LispEnvironment::parent() const
{
    return m_pParent;
}

inline
void
lcpp::LispEnvironment::name(Ptr<LispSymbol> pName)
{
    m_pName = pName;
}

inline
lcpp::Ptr<const lcpp::LispSymbol>
lcpp::LispEnvironment::name() const
{
    return m_pName;
}

inline
lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispEnvironment::name()
{
    return m_pName;
}

inline
ezString
lcpp::LispEnvironment::qualifiedName() const
{
    if(!m_pParent) { return "/"; }

    ezStringBuilder builder;
    qualifiedNameHelper(builder);
    return builder;
}

inline
ezString
lcpp::LispEnvironment::toString() const
{
    return qualifiedName();
}

inline
const lcpp::Type&
lcpp::LispEnvironment::type() const
{
    return LispEnvironment::typeInfo();
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispEnvironment::copy() const
{
    return LispEnvironment::createCopy(*this);
}

inline
bool
lcpp::operator==(const LispEnvironment& lhs, const LispEnvironment& rhs)
{
    return lhs.qualifiedName().IsEqual(rhs.qualifiedName().GetData());
}
