#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

#if 0

LCPP_TestGroup(StackPtr);

LCPP_TestCase(StackPtr, Basics)
{
    GarbageCollector::CInfo gcCinfo;

    gcCinfo.m_uiNumPages = 120 * 1024; // 120 KiB
    gcCinfo.m_pParentAllocator = defaultAllocator();

    GarbageCollector gc(gcCinfo);

    CUT_ASSERT.isTrue(gc.getNumStackReferences() == 0);

    {
        auto pObject = gc.create<LispObject>(nil::getMetaInfo()); // local pseudo nil
        CUT_ASSERT.isTrue(gc.getNumStackReferences() == 1);
    }

    CUT_ASSERT.isTrue(gc.getNumStackReferences() == 0);
}

#endif
