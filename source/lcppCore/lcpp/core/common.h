#pragma once

#ifdef LCPP_EXPORT_CORE
#define LCPP_API_CORE __declspec(dllexport)
#else
#define LCPP_API_CORE __declspec(dllimport)
#endif // LCPP_EXPORT_CORE

#include "lcpp/core/dependencies/ezEngine.h"
