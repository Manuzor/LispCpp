#pragma once

#ifdef CUT_EXPORTS
#define CUT_API __declspec(dllexport)
#else
#define CUT_API __declspec(dllimport)
#endif // CUT_EXPORTS

#include "cut/types.h"
#include "cut/preprocessor.h"
#include "cut/version.h"
#include "cut/logging/logging.h"
#include "cut/debug-break.h"
