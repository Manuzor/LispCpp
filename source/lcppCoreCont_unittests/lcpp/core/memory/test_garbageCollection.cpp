#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace
{
    class TestType : public lcpp::CollectableBase
    {
    public:
        TestType() : m_iData(0) {}

    public:
        ezInt32 m_iData;
    };
}

LCPP_TestGroup(GarbageCollection);

#if 0

LCPP_TestCase(GarbageCollection, FixedMemory)
{
    // No memory.
    {
        FixedMemory stack;

        CUT_ASSERT.isTrue(stack.getAllocationPointer() == 0, "Make sure nothing is allocated for a default-constructed instance.");
        CUT_ASSERT.isTrue(stack.getMemory().getData() == nullptr);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == 0);
        CUT_ASSERT.isTrue(stack.getEntireMemory() == stack.getMemory());
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 0);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == 0);

        auto* pTemp = (byte_t*)123;
        auto allocationResult = stack.allocate(pTemp);

        CUT_ASSERT.isTrue(allocationResult.isOutOfMemory());
        CUT_ASSERT.isTrue(pTemp == nullptr, "allocate MUST set the out_ value!");
    }

    {
        byte_t rawMemory[1024];
        Array<byte_t> entireMemory(rawMemory);
        FixedMemory stack(entireMemory);

        ezInt32* pInteger = nullptr;
        AllocatorResult result;

        //////////////////////////////////////////////////////////////////////////
        result = stack.allocate(pInteger);

        CUT_ASSERT.isTrue(result.succeeded());
        CUT_ASSERT.isTrue(stack.getAllocationPointer() == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == stack.getAllocationPointer(), "This should always be true!");
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == sizeof(ezUInt32));
        CUT_ASSERT.isTrue(stack.getAvailableMemorySize() == 1024 - stack.getStats().m_uiAllocatedSize);

        //////////////////////////////////////////////////////////////////////////
        result = stack.allocate(pInteger);

        CUT_ASSERT.isTrue(result.succeeded());
        CUT_ASSERT.isTrue(stack.getAllocationPointer() == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == stack.getAllocationPointer(), "This should always be true!");
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 2);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == sizeof(ezUInt32) * 2);
        CUT_ASSERT.isTrue(stack.getAvailableMemorySize() == 1024 - stack.getStats().m_uiAllocatedSize);

        //////////////////////////////////////////////////////////////////////////
        stack.clear();

        CUT_ASSERT.isTrue(stack.getAllocationPointer() == 0);
        CUT_ASSERT.isTrue(stack.getMemory().getData() == nullptr);
        CUT_ASSERT.isTrue(stack.getMemory().getSize() == 0);
        CUT_ASSERT.isTrue(stack.getEntireMemory() == stack.getMemory());
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocations == 0);
        CUT_ASSERT.isTrue(stack.getStats().m_uiAllocatedSize == 0);
    }

    {
        char raw0[6];
        char raw1[6];

        strcpy_s(raw0, "hello");
        strcpy_s(raw1, "world");

        Array<byte_t> buffer0(reinterpret_cast<byte_t*>(raw0), 6);
        Array<byte_t> buffer1(reinterpret_cast<byte_t*>(raw1), 6);

        FixedMemory mem0(buffer0);
        FixedMemory mem1(buffer1);

        ezInt16* ptr0;
        ezInt32* ptr1;

        CUT_ASSERT.isTrue(mem0.allocate(ptr0).succeeded());
        CUT_ASSERT.isTrue(mem1.allocate(ptr1).succeeded());

        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocatedSize == sizeof(ezInt16));
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocatedSize == sizeof(ezInt32));

        auto temp = mem0;
        mem0 = mem1;
        mem1 = temp;

        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem0.getStats().m_uiAllocatedSize == sizeof(ezInt32));
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocations == 1);
        CUT_ASSERT.isTrue(mem1.getStats().m_uiAllocatedSize == sizeof(ezInt16));
    }
}

#endif

LCPP_TestCaseNoInit(GarbageCollection, Basics)
{
    GarbageCollector::CInfo gcCinfo;

    gcCinfo.m_uiNumInitialPages = 1;
    gcCinfo.m_pParentAllocator = defaultAllocator();

    GarbageCollector gc;
    gc.initialize(gcCinfo);
    gc.clear();
    gc.initialize(gcCinfo);
    gc.clear();
}
