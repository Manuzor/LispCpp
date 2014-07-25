#pragma once

#ifdef LCPP_EXPORT_CORE_CONT
#define LCPP_API_CORE_CONT __declspec(dllexport)
#else
#define LCPP_API_CORE_CONT __declspec(dllimport)
#endif // LCPP_EXPORT_FOUNDATION

#include "lcpp/core/config.h"

#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK_ENABLED)
#define LCPP_RUNTIME_TYPE_CHECK EZ_ASSERT_ALWAYS
#else
#define LCPP_RUNTIME_TYPE_CHECK(...)
#endif
