#pragma once
#include <tchar.h>
#include <string>
#include <sstream>

#ifdef LCPP_CORE_EXPORT
#define LCPP_CORE_API __declspec(dllexport)
#else
#define LCPP_CORE_API __declspec(dllimport)
#endif // LCPP_CORE_EXPORT

namespace lcpp
{
    typedef std::wstring String;
    typedef std::wstringstream StringStream;
}
