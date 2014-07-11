
inline
    lcpp::SchemeString::SchemeString(const char* str) :
    m_string(str)
{
}

inline
lcpp::SchemeString::SchemeString(const ezString& str) :
    m_string(str)
{
}

inline
lcpp::SchemeString::~SchemeString()
{
}

inline
ezString
lcpp::SchemeString::toString() const
{
    ezStringBuilder builder(m_string.GetIteratorFront());
    builder.Prepend('"');
    builder.Append('"');
    return builder;
}

inline
lcpp::SchemeString::operator ezString() const
{
    return m_string;
}

inline
bool
lcpp::SchemeString::operator ==(const SchemeObject& rhs) const
{
    if(rhs.is<SchemeString>())
    {
        return *this == static_cast<const SchemeString&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::SchemeString::value() const
{
    return m_string;
}

inline
void
lcpp::SchemeString::value(const ezString& newSymbol)
{
    m_string = newSymbol;
}

inline
void
lcpp::SchemeString::value(const char* newSymbol)
{
    m_string = newSymbol;
}

inline
bool
lcpp::operator ==(const SchemeString& lhs, const SchemeString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const SchemeString& lhs, const char* rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs);
}

inline
bool
lcpp::operator ==(const char* lhs, const SchemeString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs,
        rhs.value().GetData());
}

inline
bool
lcpp::operator ==(const SchemeString& lhs, const ezString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.GetData());
}

inline
bool
lcpp::operator ==(const ezString& lhs, const SchemeString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.GetData(),
        rhs.value().GetData());
}
