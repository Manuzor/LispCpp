
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
lcpp::SchemeSymbol::operator ezString() const
{
    return m_symbol;
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
lcpp::SchemeSymbol::value() const
{
    return m_symbol;
}

inline
void
lcpp::SchemeSymbol::value(const ezString& newSymbol)
{
    m_symbol = newSymbol;
}

inline
void
lcpp::SchemeSymbol::value(const char* newSymbol)
{
    m_symbol = newSymbol;
}

inline
bool
lcpp::operator ==(const SchemeSymbol& lhs, const SchemeSymbol& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const SchemeSymbol& lhs, const char* rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs);
}

inline
bool
lcpp::operator ==(const char* lhs, const SchemeSymbol& rhs)
{
    return ezStringUtils::IsEqual(
        lhs,
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const SchemeSymbol& lhs, const ezString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.GetData());
}

inline
bool
lcpp::operator ==(const ezString& lhs, const SchemeSymbol& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.GetData(),
        rhs.value().GetData());
}
