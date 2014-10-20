#pragma once

#ifdef CUT_EXPORTS
#define CUT_API __declspec(dllexport)
#else
#define CUT_API __declspec(dllimport)
#endif // CUT_EXPORTS

#define CUT_INLINE inline

#ifdef _WIN32
#define CUT_FORCE_INLINE __forceinline
#else
#define CUT_FORCE_INLINE inline
#endif

#include "cut/types.h"
#include "cut/preprocessor.h"
#include "cut/version.h"
#include "cut/logging/logging.h"
#include "cut/debug-break.h"
