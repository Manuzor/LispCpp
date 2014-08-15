#include "stdafx.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    Stack::Stack() :
        StackBase(defaultAllocator())
    {
        setNilElement(LCPP_pNil);
    }
}
