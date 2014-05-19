#pragma once

namespace lcpp
{
    LCPP_CORE_API ezAllocatorBase* defaultAllocator();

    class LCPP_CORE_API MallocAllocator : public ezAllocatorBase
    {
    public:
        MallocAllocator();
        virtual ~MallocAllocator();

        void* Allocate(size_t size, size_t alignment) LCPP_OVERRIDE;
        void Deallocate(void* pMemory) LCPP_OVERRIDE;

        virtual size_t AllocatedSize(const void* ptr) LCPP_OVERRIDE;
        virtual Stats GetStats() const LCPP_OVERRIDE;
    };
}
