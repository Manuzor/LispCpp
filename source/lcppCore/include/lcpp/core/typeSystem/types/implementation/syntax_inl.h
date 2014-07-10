
inline
const lcpp::Type&
lcpp::SchemeSyntax::type() const
{
    return TypeInfo<SchemeSyntax>::type();
}

inline
void
lcpp::SchemeSyntax::name(Ptr<SchemeSymbol> pName)
{
    m_pName = pName;
}

inline
lcpp::Ptr<lcpp::SchemeSymbol>
lcpp::SchemeSyntax::name()
{
    return m_pName;
}

inline
lcpp::Ptr<const lcpp::SchemeSymbol>
lcpp::SchemeSyntax::name() const
{
    return m_pName;
}
