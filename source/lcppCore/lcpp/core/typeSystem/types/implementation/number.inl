﻿inline
lcpp::LispInteger::LispInteger(Number_t value) :
    m_value(value)
{
}

inline
lcpp::LispInteger::LispInteger(const LispInteger& toCopy) :
    m_value(toCopy.m_value)
{
}

inline
bool
lcpp::LispInteger::operator ==(const LispObject& obj) const
{
    if(obj.is<LispInteger>())
    {
        return *this == static_cast<const LispInteger&>(obj); // Call the specialized operator ==
    }
    return false;
}

inline
lcpp::LispInteger::Number_t
lcpp::LispInteger::value(void) const
{
    return m_value;
}

inline
void
lcpp::LispInteger::value(Number_t val)
{
    m_value = val;
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispInteger::copy() const
{
    return create(m_value);
}

inline
const lcpp::Type&
lcpp::LispInteger::type() const
{
    return LispInteger::typeInfo();
}

inline
bool
lcpp::operator==(const LispInteger& lhs, const LispInteger& rhs)
{
    return lhs.value() == rhs.value();
}

//////////////////////////////////////////////////////////////////////////

inline
lcpp::LispNumber::LispNumber(Number_t value) :
    m_value(value)
{
}

inline
lcpp::LispNumber::LispNumber(const LispNumber& toCopy) :
    m_value(toCopy.m_value)
{
}

inline
bool
lcpp::LispNumber::operator ==(const LispObject& obj) const
{
    if(obj.is<LispNumber>())
    {
        return *this == static_cast<const LispNumber&>(obj); // Call the specialized operator ==
    }
    return false;
}

inline
lcpp::LispNumber::Number_t
lcpp::LispNumber::value(void) const
{
    return m_value;
}

inline
void
lcpp::LispNumber::value(Number_t val)
{
    m_value = val;
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispNumber::copy() const
{
    return create(m_value);
}

inline
const lcpp::Type&
lcpp::LispNumber::type() const
{
    return LispNumber::typeInfo();
}

inline
bool
lcpp::operator==(const LispNumber& lhs, const LispNumber& rhs)
{
    return lhs.value() == rhs.value();
}

inline
bool
lcpp::operator==(const LispInteger& lhs, const LispNumber& rhs)
{
    return lhs.value() == rhs.value();
}

inline
bool
lcpp::operator==(const LispNumber& lhs, const LispInteger& rhs)
{
    return lhs.value() == rhs.value();
}
