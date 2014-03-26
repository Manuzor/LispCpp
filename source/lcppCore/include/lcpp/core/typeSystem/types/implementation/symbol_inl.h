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
bool
lcpp::SchemeSymbol::operator ==(const SchemeObject& rhs) const
{
    if (rhs.is(TypeInfo<SchemeSymbol>::type()))
    {
        return *this == static_cast<const SchemeSymbol&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::SchemeSymbol::symbol() const
{
    return m_symbol;
}

inline
void
lcpp::SchemeSymbol::symbol(const ezString& newSymbol)
{
    m_symbol = newSymbol;
}

inline
void
lcpp::SchemeSymbol::symbol(const char* newSymbol)
{
    m_symbol = newSymbol;
}

inline
bool
lcpp::operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs)
{
    return lhs.symbol() == lhs.symbol();
}
