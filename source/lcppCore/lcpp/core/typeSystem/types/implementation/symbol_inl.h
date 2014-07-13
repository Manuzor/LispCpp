
inline
lcpp::LispSymbol::LispSymbol(const char* symbol) :
    m_symbol(symbol)
{
}

inline
lcpp::LispSymbol::LispSymbol(const ezString& symbol) :
    m_symbol(symbol)
{
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispSymbol::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, LispSymbol)(m_symbol);
}

inline
const lcpp::Type&
lcpp::LispSymbol::type() const
{
    return LispSymbol::typeInfo();
}

inline
ezString
lcpp::LispSymbol::toString() const
{
    return m_symbol;
}

inline
bool
lcpp::LispSymbol::operator ==(const LispObject& rhs) const
{
    if(rhs.is<LispSymbol>())
    {
        return *this == static_cast<const LispSymbol&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::LispSymbol::value() const
{
    return m_symbol;
}

inline
bool
lcpp::operator ==(const LispSymbol& lhs, const LispSymbol& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.value().GetData());
}
