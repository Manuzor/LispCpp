#include "stdafx.h"
#include "lcpp/core/memory/stackPtr.h"

namespace lcpp
{
    CollectableBase* volatile StackPtrBase::s_ptrTable[StackPtrBase::NumMaxStackPtrs];
    ezUInt32 StackPtrBase::s_uiNextIndex(0);
}
