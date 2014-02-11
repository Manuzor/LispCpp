#pragma once

#ifdef LCPP_CORE_EXPORT
#define LCPP_CORE_API __declspec(dllexport)
#else
#define LCPP_CORE_API __declspec(dllimport)
#endif // LCPP_CORE_EXPORT

// ezEngine stuff
#include <Foundation/Basics/IncludeAll.cpp>

#include "lcpp/foundation/newdelete.h"

#define LCPP_NEW(type)               EZ_DEFAULT_NEW(type)
#define LCPP_NEW_ARRAY(type)         EZ_DEFAULT_NEW_ARRAY(type)
#define LCPP_NEW_RAW_BUFFER(type)    EZ_DEFAULT_NEW_RAW_BUFFER(type)

#define LCPP_DELETE(type)            EZ_DEFAULT_DELETE(type)
#define LCPP_DELETE_ARRAY(type)      EZ_DEFAULT_DELETE_ARRAY(type)
#define LCPP_DELETE_RAW_BUFFER(type) EZ_DEFAULT_DELETE_RAW_BUFFER(type)

#include "lcpp/exceptions/exceptions.h"

#define LCPP_DISALLOW_CONSTRUCTION(type) EZ_DISALLOW_COPY_AND_ASSIGN(type); type(); ~type()
