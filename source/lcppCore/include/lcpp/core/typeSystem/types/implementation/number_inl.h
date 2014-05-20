#include "lcpp/foundation/conversion.h"

template<typename NUMBER_TYPE>
inline
lcpp::SchemeNumber_t<NUMBER_TYPE>::SchemeNumber_t(Number_t value) :
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
    if(obj.is<SchemeNumber_t<NUMBER_TYPE>>())
    {
        return *this == static_cast<const SchemeNumber_t<Number_t>&>(obj); // Call the specialized operator ==
    }
    return false;
}

template<typename NUMBER_TYPE>
inline
typename NUMBER_TYPE
lcpp::SchemeNumber_t<NUMBER_TYPE>::value(void) const
{
    return m_value;
}

template<typename NUMBER_TYPE>
inline
void
lcpp::SchemeNumber_t<NUMBER_TYPE>::value(NUMBER_TYPE val)
{
    m_value = val;
}

template<typename NUMBER_TYPE>
inline
ezString
lcpp::SchemeNumber_t<NUMBER_TYPE>::toString() const
{
    return lcpp::toString(m_value);
}
