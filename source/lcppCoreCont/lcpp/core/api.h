#pragma once

#include "lcpp/foundation/api.h"

#include "lcpp/core/config.h"
#include "lcpp/core/dependencies.h"
#include "lcpp/core/common.h"

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG) || EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
#include "lcpp/core/typeSystem/allTypeData.h"
#endif
