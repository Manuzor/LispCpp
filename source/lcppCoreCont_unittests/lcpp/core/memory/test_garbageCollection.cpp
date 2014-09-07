#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/types/symbol.h"

namespace lcpp
{
    namespace test
    {
        namespace
        {
            class TestAllocator :
                public ezAllocatorBase
            {
            public:
                struct Allocation
                {
                    EZ_DECLARE_POD_TYPE();

                    void* m_ptr;
                    std::size_t m_uiSize;
                    std::size_t m_uiAlign;
                };

            public:
                ezHashTable<void*, Allocation> m_allocations;
                ezHashTable<void*, Allocation> m_deallocations;
                Array<byte_t> m_protectedMemoryLeft;
                Array<byte_t> m_usableMemory;
                Array<byte_t> m_protectedMemoryRight;
                std::size_t m_uiAllocationPointer;

            public:
                TestAllocator() :
                    m_uiAllocationPointer(0)
                {
                }

                TestAllocator(std::size_t uiProtectedBytesLeft,
                              std::size_t uiUnprotectedSize,
                              std::size_t uiProtectedBytesRight) :
                    m_uiAllocationPointer(0)
                {
                    Initialize(uiProtectedBytesLeft, uiUnprotectedSize, uiProtectedBytesRight);
                }

                ~TestAllocator()
                {
                    Clear();
                }

                void Initialize(std::size_t uiProtectedBytesLeft,
                              std::size_t uiUnprotectedSize,
                              std::size_t uiProtectedBytesRight)
                {
                    const auto uiSize = uiProtectedBytesLeft
                                      + uiUnprotectedSize
                                      + uiProtectedBytesRight;

                    if (uiSize == 0)
                    {
                        return;
                    }

                    auto pMemory = (byte_t*)malloc(uiSize);
                    auto pUsableMemory = pMemory + uiProtectedBytesLeft;
                    auto pProtectedMemoryRight = pUsableMemory + uiProtectedBytesRight;

                    m_protectedMemoryLeft.assign(pMemory, uiProtectedBytesLeft);
                    m_usableMemory.assign(pUsableMemory, uiUnprotectedSize);
                    m_protectedMemoryRight.assign(pProtectedMemoryRight, uiProtectedBytesRight);

                    memset(m_protectedMemoryLeft.getData(),  Byte::Protected,   m_protectedMemoryLeft.getSize());
                    memset(m_usableMemory.getData(),         Byte::Unallocated, m_usableMemory.getSize());
                    memset(m_protectedMemoryRight.getData(), Byte::Protected,   m_protectedMemoryRight.getSize());

                    // At this point, the memory looks like this:
                    // 0x 0ace0ace0ace0ac baaabaaabaaabaaab 0ace0ace0ace0ac
                    //    \_ Protected _/ \_ User Memory _/ \_ Protected _/
                }

                void Clear()
                {
                    if (m_allocations.GetCount() != m_deallocations.GetCount())
                    {
                        CUT_ASSERT.fail("Not enough frees!");
                        return;
                    }

                    free(m_protectedMemoryLeft.getData());
                }

                virtual void* Allocate(std::size_t uiSize, std::size_t uiAlign) override
                {
                    if (uiSize == 0 || uiSize == 42)
                    {
                        return nullptr;
                    }

                    if (m_uiAllocationPointer + uiSize > m_usableMemory.getSize())
                    {
                        // Out of memory!
                        return nullptr;
                    }

                    Allocation allocation{ nullptr, uiSize, uiAlign };
                    allocation.m_ptr = &m_usableMemory[m_uiAllocationPointer];
                    m_uiAllocationPointer += allocation.m_uiSize;

                    memset(allocation.m_ptr, Byte::Allocated, allocation.m_uiSize);

                    m_allocations[allocation.m_ptr] = allocation;

                    return allocation.m_ptr;
                }

                virtual void Deallocate(void* ptr) override
                {
                    if (ptr == nullptr)
                    {
                        return;
                    }

                    if (m_deallocations.KeyExists(ptr))
                    {
                        CUT_ASSERT.fail("Double free!");
                        return;
                    }

                    Allocation* pAllocation(nullptr);

                    if (!m_allocations.TryGetValue(ptr, pAllocation))
                    {
                        CUT_ASSERT.fail("Invalid free!");
                        return;
                    }

                    memset(pAllocation->m_ptr, Byte::Freed, pAllocation->m_uiSize);

                    m_deallocations[ptr] = *pAllocation;
                }

                virtual size_t AllocatedSize(const void*) override
                {
                    return 0;
                }

                virtual Stats GetStats() const override
                {
                    return Stats();
                }

            };
        }
    }
}

LCPP_TestGroup(GarbageCollection);

LCPP_TestCase(GarbageCollection, MemoryStack_Basics)
{
    // Note: This allocator is always out of memory when no arguments are given!
    TestAllocator allocator;

    MemoryStack stack(&allocator);

    {
        MemoryStack mem2;
        CUT_ASSERT.isTrue(mem2.getAllocator().isNull());
        mem2.setAllocator(&allocator);
        CUT_ASSERT.isTrue(stack.getAllocator() == mem2.getAllocator());
    }

    byte_t* pMemory(nullptr);
    MemoryStack::AllocationResult result;

    // Allocate 0 bytes.
    result = stack.allocate(pMemory, 0);

    CUT_ASSERT.isTrue(result.nothingChanged());
    CUT_ASSERT.isTrue(pMemory == nullptr);

    // Allocate 42 bytes, which is a special size the test-allocator expects in order to return nullptr.
    result = stack.allocate(pMemory, 42);

    CUT_ASSERT.isTrue(result.isOutOfMemory());
    CUT_ASSERT.isTrue(pMemory == nullptr);
}

LCPP_TestCase(GarbageCollection, MemoryStack_Allocation)
{
    TestAllocator allocator(1, 1, 1);
    MemoryStack stack(&allocator);
    stack.resize(1);

    byte_t* pMemory(nullptr);
    MemoryStack::AllocationResult result;
    MemoryStack::Stats stats;

    //////////////////////////////////////////////////////////////////////////

    result = stack.allocate(pMemory, 0);
    stats = stack.getStats();

    CUT_ASSERT.isTrue(result.nothingChanged());
    CUT_ASSERT.isTrue(pMemory == nullptr);
    CUT_ASSERT.isTrue(stats.m_uiAllocations == 0);

    //////////////////////////////////////////////////////////////////////////

    result = stack.allocate(pMemory, 1);
    stats = stack.getStats();

    CUT_ASSERT.isTrue(result.succeeded());
    CUT_ASSERT.isTrue(pMemory != nullptr);
    CUT_ASSERT.isTrue(*(pMemory - 1) == Byte::Protected);
    CUT_ASSERT.isTrue(*pMemory == Byte::Allocated);
    CUT_ASSERT.isTrue(*(pMemory + 1) == Byte::Protected);
    CUT_ASSERT.isTrue(stats.m_uiAllocations == 1);

    CUT_ASSERT.notImplemented("There are still a lot of tests missing!");
}

LCPP_TestCase(GarbageCollection, MemoryStack_Resize)
{
    TestAllocator allocator(32, 2048, 32);
    MemoryStack stack(&allocator);
    MemoryStackAllocator wrapper(&stack);

    stack.resize(600);

    CUT_ASSERT.notImplemented();
}
