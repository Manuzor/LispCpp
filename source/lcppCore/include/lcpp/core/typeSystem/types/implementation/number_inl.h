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
bool
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeTypeInfo<SchemeNumber_t<NUMBER_TYPE>>::type()))
    {
        return *this == static_cast<const type_t&>(obj); // Call the specialized operator ==
    }
    return false;
}

template<typename NUMBER_TYPE>
inline
bool
lcpp::SchemeNumber_t<NUMBER_TYPE>::operator ==(const type_t& other) const
{
    return m_value == other.m_value;
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
