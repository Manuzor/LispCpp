#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"

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

            public:
                ~TestAllocator()
                {
                    Clear();
                }

                void Clear()
                {
                    if (m_allocations.GetCount() != m_deallocations.GetCount())
                    {
                        CUT_ASSERT.fail("Not enough frees!");
                        return;
                    }

                    for (auto iter = m_allocations.GetIterator(); iter.IsValid(); ++iter)
                    {
                        free(iter.Key());
                    }
                }

                virtual void* Allocate(size_t uiSize, size_t uiAlign) override
                {
                    if (uiSize == 0 || uiSize == 42)
                    {
                        return nullptr;
                    }

                    Allocation allocation{ nullptr, uiSize, uiAlign };

                    allocation.m_ptr = malloc(uiSize);

                    if (allocation.m_ptr == nullptr)
                    {
                        // Out of real memory! o_o
                        LCPP_NOT_IMPLEMENTED;
                    }

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

                    if (m_allocations.TryGetValue(ptr, pAllocation))
                    {
                        CUT_ASSERT.fail("Invalid free!");
                        return;
                    }

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

LCPP_TestCase(GarbageCollection, MemoryStack)
{
    TestAllocator allocator;
    MemoryStack mem(&allocator);

    {
        MemoryStack mem2;
        CUT_ASSERT.isTrue(mem2.getAllocator().isNull());
        mem2.setAllocator(&allocator);
        CUT_ASSERT.isTrue(mem.getAllocator() == mem2.getAllocator());
    }

    byte_t* pMemory(nullptr);

    // Allocate 42, which is a special size the test-allocator expects in order to return nullptr.
    auto result = mem.allocate(pMemory, 42);

    CUT_ASSERT.isTrue(result.isOutOfMemory());
    CUT_ASSERT.isTrue(pMemory == nullptr);

    // Allocate 0 bytes.
    result = mem.allocate(pMemory, 0);

    CUT_ASSERT.isTrue(result.succeeded());
    CUT_ASSERT.isTrue(pMemory == nullptr);

    CUT_ASSERT.notImplemented("There are still a lot of tests missing!");
}
