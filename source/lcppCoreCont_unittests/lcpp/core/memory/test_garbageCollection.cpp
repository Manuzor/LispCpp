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
                virtual void* Allocate(size_t uiSize, size_t uiAlign) override
                {
                    switch(uiSize)
                    {
                    case 42:
                        return nullptr;
                    default:
                        return malloc(uiSize);
                    }

                    EZ_REPORT_FAILURE("This point should never be reached.");
                    return nullptr;
                }

                virtual void Deallocate(void* ptr) override
                {
                    if (ptr == nullptr)
                    {
                        return;
                    }

                    free(ptr);
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
    MemoryStack mem(defaultAllocator());
    
    byte_t* pMemory(nullptr);
    mem.allocate(pMemory, 42);
    
    CUT_ASSERT.isTrue(pMemory == nullptr);
}
