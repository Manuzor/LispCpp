#include "stdafx.h"
#include "lcpp/foundation/memory/allocator.h"

ezAllocatorBase* lcpp::defaultAllocator()
{
    static MallocAllocator allocator;
    return &allocator;
}

lcpp::MallocAllocator::MallocAllocator()
{
}

lcpp::MallocAllocator::~MallocAllocator()
{
}

void* lcpp::MallocAllocator::Allocate(size_t size, size_t alignment)
{
    LCPP_UNUSED(alignment);
    return malloc(size);
}

void lcpp::MallocAllocator::Deallocate(void* pMemory)
{
    free(pMemory);
}

size_t lcpp::MallocAllocator::AllocatedSize(const void* ptr)
{
    return 0;
}

ezAllocatorBase::Stats lcpp::MallocAllocator::GetStats() const
{
    return ezAllocatorBase::Stats();
}
