#ifndef LCPP_CORE_CONT_INCLUDE_ALL
#error Do not include this file directly in your project! It is meant for internal use only.
#endif

#include "stdafx.h"

#include "lcpp/core/common.h"
#include "lcpp/core/config.h"
#include "lcpp/core/containers/instanceTable.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/continuation.h"
#include "lcpp/core/exceptions/runtimeException.h"
#include "lcpp/core/exceptions/typeCheckFailedException.h"
#include "lcpp/core/recursionCounter.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/sourceCursor.h"
#include "lcpp/core/typeSystem/includeAllTypes.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/numberHelpers.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/void.h"
