
inline
const lcpp::Type&
lcpp::SchemeSyntax::type() const
{
    return TypeInfo<SchemeSyntax>::type();
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
