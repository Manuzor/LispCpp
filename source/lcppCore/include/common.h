#pragma once

#ifdef LCPP_CORE_EXPORT
#define LCPP_CORE_API __declspec(dllexport)
#else
#define LCPP_CORE_API __declspec(dllimport)
#endif // LCPP_CORE_EXPORT

// ezEngine stuff
#include <Foundation/Basics/IncludeAll.cpp>

#include "exceptions/exceptions.h"

#define DISALLOW_COPY_AND_ASSIGNMENT(type) private:\
    type(const type&);\
    void operator=(const type&)
