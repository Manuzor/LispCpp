#include "lcpp/foundation/conversion.h"

template<typename NUMBER_TYPE>
inline
lcpp::LispNumber_t<NUMBER_TYPE>::LispNumber_t(Number_t value) :
    m_value(value)
{
}

template<typename NUMBER_TYPE>
inline
lcpp::LispNumber_t<NUMBER_TYPE>::~LispNumber_t()
{
}

template<typename NUMBER_TYPE>
inline
bool
lcpp::LispNumber_t<NUMBER_TYPE>::operator ==(const LispObject& obj) const
{
    if(obj.is<LispNumber_t<NUMBER_TYPE>>())
    {
        return *this == static_cast<const LispNumber_t<Number_t>&>(obj); // Call the specialized operator ==
    }
    return false;
}

template<typename NUMBER_TYPE>
inline
typename NUMBER_TYPE
lcpp::LispNumber_t<NUMBER_TYPE>::value(void) const
{
    return m_value;
}

template<typename NUMBER_TYPE>
inline
void
lcpp::LispNumber_t<NUMBER_TYPE>::value(NUMBER_TYPE val)
{
    m_value = val;
}

template<typename NUMBER_TYPE>
inline
ezString
lcpp::LispNumber_t<NUMBER_TYPE>::toString() const
{
    return lcpp::toString(m_value);
}
