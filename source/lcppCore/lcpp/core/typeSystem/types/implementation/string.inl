
inline
    lcpp::LispString::LispString(const char* str) :
    m_string(str)
{
}

inline
lcpp::LispString::LispString(const ezString& str) :
    m_string(str)
{
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispString::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, LispString)(m_string);
}

inline
const lcpp::Type&
lcpp::LispString::type() const
{
    return LispString::typeInfo();
}

inline
ezString
lcpp::LispString::toString() const
{
    ezStringBuilder builder(m_string.GetIteratorFront());
    builder.Prepend('"');
    builder.Append('"');
    return builder;
}

inline
lcpp::LispString::operator ezString() const
{
    return m_string;
}

inline
bool
lcpp::LispString::operator ==(const LispObject& rhs) const
{
    if(rhs.is<LispString>())
    {
        return *this == static_cast<const LispString&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::LispString::value() const
{
    return m_string;
}

inline
void
lcpp::LispString::value(const ezString& newSymbol)
{
    m_string = newSymbol;
}

inline
void
lcpp::LispString::value(const char* newSymbol)
{
    m_string = newSymbol;
}

inline
bool
lcpp::operator ==(const LispString& lhs, const LispString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const LispString& lhs, const char* rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs);
}

inline
bool
lcpp::operator ==(const char* lhs, const LispString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs,
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const LispString& lhs, const ezString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.GetData());
}

inline
bool
lcpp::operator ==(const ezString& lhs, const LispString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.GetData(),
        rhs.value().GetData());
}
