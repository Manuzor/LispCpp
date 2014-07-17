
inline
const lcpp::Type&
lcpp::LispSyntax::type() const
{
    return LispSyntax::typeInfo();
}

inline
void
lcpp::LispSyntax::name(Ptr<LispSymbol> pName)
{
    m_pName = pName;
}

inline
lcpp::Ptr<lcpp::LispSymbol>
lcpp::LispSyntax::name()
{
    return m_pName;
}

inline
lcpp::Ptr<const lcpp::LispSymbol>
lcpp::LispSyntax::name() const
{
    return m_pName;
}
