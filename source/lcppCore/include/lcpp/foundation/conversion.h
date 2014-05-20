#pragma once
#include <string>

namespace lcpp
{
    namespace detail
    {
        template<typename T_Target>
        struct ConvertTo {};
        // Special case for ezString. The convert function never fails.

        template<>
        struct ConvertTo<ezString>
        {
            template<typename T>
            inline static ezString convert(const T& value)
            {
                return ezConversionUtils::ToString(value);
            }
        };

        template<>
        struct ConvertTo<ezInt64>
        {
            inline static ezResult convert(const ezString& str, ezInt64& out, const char** out_lastParsePos)
            {
                return ezConversionUtils::StringToInt64(str.GetData(), out, out_lastParsePos);
            }
            inline static ezResult convert(const ezStringIterator& iter, ezInt64& out, const char** out_lastParsePos)
            {
                return ezConversionUtils::StringToInt64(iter.GetData(), out, out_lastParsePos);
            }
        };

        template<>
        struct ConvertTo<double>
        {
            inline static ezResult convert(const ezString& str, double& out, const char** out_lastParsePos = nullptr)
            {
                return ezConversionUtils::StringToFloat(str.GetData(), out, out_lastParsePos);
            }
            inline static ezResult convert(const ezStringIterator& iter, double& out, const char** out_lastParsePos = nullptr)
            {
                return ezConversionUtils::StringToFloat(iter.GetData(), out, out_lastParsePos);
            }
        };
    }

    template<typename T_To, typename T_From>
    inline
    ezResult to(const T_From& from, T_To& out_to)
    {
        return detail::ConvertTo<T_To>::convert(from, out_to);
    }

    template<typename T_To>
    inline
    ezResult to(const ezString& from, T_To& out_to, const char** out_lastParsePos = nullptr)
    {
        return detail::ConvertTo<T_To>::convert(from, out_to, out_lastParsePos);
    }

    template<typename T_To>
    inline
    ezResult to(const ezStringIterator& from, T_To& out_to, const char** out_lastParsePos = nullptr)
    {
        return detail::ConvertTo<T_To>::convert(from, out_to, out_lastParsePos);
    }

    template<typename T>
    inline
    ezString toString(T value)
    {
        return detail::ConvertTo<ezString>::convert(value);
    }
}
