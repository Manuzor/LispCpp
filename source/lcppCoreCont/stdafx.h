#pragma once

#include <SDKDDKVer.h>

#include "lcpp/foundation/api.h"

#include "lcpp/core/config.h"
#include "lcpp/core/dependencies.h"

#if EZ_ENABLED(LCPP_VERBOSE_LOGGING)
// Enable this to allow verbose debug messages
#define VerboseDebugMessage Debug
#endif

#include "lcpp/core/common.h"
