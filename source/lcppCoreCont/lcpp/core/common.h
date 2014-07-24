#pragma once

#ifdef LCPP_EXPORT_CORE_CONT
#define LCPP_API_CORE_CONT __declspec(dllexport)
#else
#define LCPP_API_CORE_CONT __declspec(dllimport)
#endif // LCPP_EXPORT_FOUNDATION
