
inline
    lcpp::SchemeSymbol::SchemeSymbol(const char* symbol) :
    m_symbol(symbol)
{
}

inline
lcpp::SchemeSymbol::SchemeSymbol(const ezString& symbol) :
    m_symbol(symbol)
{
}

inline
lcpp::SchemeSymbol::~SchemeSymbol()
{
}

inline
ezString
lcpp::SchemeSymbol::toString() const
{
    return m_symbol;
}

inline
bool
lcpp::SchemeSymbol::operator ==(const SchemeObject& rhs) const
{
    if(rhs.is<SchemeSymbol>())
    {
        return *this == static_cast<const SchemeSymbol&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::SchemeSymbol::value() const
{
    return m_symbol;
}

inline
bool
lcpp::operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.value().GetData());
}
