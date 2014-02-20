#include "lcpp/foundation/stringUtil.h"

template<typename NUMBER_TYPE>
inline
lcpp::SchemeNumber_t<NUMBER_TYPE>::SchemeNumber_t(number_t value) :
m_value(value)
{
}

template<typename NUMBER_TYPE>
inline
lcpp::SchemeNumber_t<NUMBER_TYPE>::~SchemeNumber_t()
{
}

template<typename NUMBER_TYPE>
inline
const lcpp::SchemeBool&
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeType::Number))
    {
        return convert(*this == static_cast<const type_t&>(obj));
    }
    return SCHEME_FALSE;
}

template<typename NUMBER_TYPE>
inline
const lcpp::SchemeBool&
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const type_t& other) const
{
    return convert(m_value == other.m_value);
}

template<typename T>
inline
T
lcpp::SchemeNumber_t<T>::value() const
{
    return m_value;
}

template<typename T>
inline
void
lcpp::SchemeNumber_t<T>::value(T value)
{
    m_value = value;
}

template<typename NUMBER_TYPE>
inline
ezString
lcpp::SchemeNumber_t<NUMBER_TYPE>::toString() const
{
    return ::lcpp::toString(m_value);
}
