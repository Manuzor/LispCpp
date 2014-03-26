#pragma once

namespace lcpp
{
    template<typename T>
    struct GetString;

    // Actual global function to convert a type to an ezString
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    ezString toString(T value)
    {
        return _helper::GetString<T>::of(value);
    }

    // Helper struct and macro
    //////////////////////////////////////////////////////////////////////////

    namespace _helper
    {
        /// \brief Base template for converting a type T to an ezString.
        template<typename T>
        struct GetString
        {
            static ezString of(T value)
            {
                static_assert(false, "Specified type is not supported for toString/GetString!");
            }
        };
    }
}

#define LCPP_GETSTRING_POD_SPECIALIZATION(type, fmt)                                                  \
namespace lcpp { namespace _helper {                                                                  \
    template<>                                                                                        \
    struct GetString<type>                                                                            \
    {                                                                                                 \
        static ezString of(type value)                                                                \
        {                                                                                             \
            static const size_t bufferSize = 32;                                                      \
            char buffer[bufferSize];                                                                  \
            auto size = ezStringUtils::snprintf(buffer, bufferSize, fmt, value);                      \
            EZ_ASSERT(size > -1, "Check the buffer for a possible error message.");                   \
            EZ_ASSERT(size <= bufferSize, "Buffer size is %u but %u is required.", bufferSize, size); \
            return buffer;                                                                            \
        }                                                                                             \
    };                                                                                                \
}}

LCPP_GETSTRING_POD_SPECIALIZATION(char, "%c");
LCPP_GETSTRING_POD_SPECIALIZATION(unsigned char, "%c");
LCPP_GETSTRING_POD_SPECIALIZATION(ezInt16, "%d");
LCPP_GETSTRING_POD_SPECIALIZATION(ezUInt16, "%u");
LCPP_GETSTRING_POD_SPECIALIZATION(ezInt32, "%d");
LCPP_GETSTRING_POD_SPECIALIZATION(ezUInt32, "%u");
LCPP_GETSTRING_POD_SPECIALIZATION(ezInt64, "%dll");
LCPP_GETSTRING_POD_SPECIALIZATION(ezUInt64, "%ull");
LCPP_GETSTRING_POD_SPECIALIZATION(float, "%f");
LCPP_GETSTRING_POD_SPECIALIZATION(double, "%f");
