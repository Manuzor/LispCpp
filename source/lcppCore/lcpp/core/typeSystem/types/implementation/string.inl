
inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispString::copy() const
{
    return create(m_string);
}

inline
const lcpp::Type&
lcpp::LispString::type() const
{
    return LispString::typeInfo();
}

inline
lcpp::String
lcpp::LispString::toString() const
{
    ezStringBuilder builder(m_string.GetIteratorFront());
    builder.Prepend('"');
    builder.Append('"');
    return builder;
}

inline
lcpp::LispString::operator lcpp::String() const
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
const lcpp::String&
lcpp::LispString::value() const
{
    return m_string;
}

inline
void
lcpp::LispString::value(const String& newSymbol)
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
lcpp::operator ==(const LispString& lhs, const String& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.value().GetData(),
        rhs.GetData());
}

inline
bool
lcpp::operator ==(const String& lhs, const LispString& rhs)
{
    return ezStringUtils::IsEqual(
        lhs.GetData(),
        rhs.value().GetData());
}
