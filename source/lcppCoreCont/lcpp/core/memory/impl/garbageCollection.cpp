#include "stdafx.h"
#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    ezAllocatorBase* getCollectingAllocator()
    {
        static CollectingAllocator allocator(defaultAllocator());
        return &allocator;
    }

    CollectingAllocator::CollectingAllocator(ezAllocatorBase* pParentAllocator) :
        m_pParent(pParentAllocator)
    {
    }

    void* CollectingAllocator::Allocate(size_t uiSize, size_t uiAlign)
    {
        return m_pParent->Allocate(uiSize, uiAlign);
    }

    void CollectingAllocator::Deallocate(void* ptr)
    {
        return m_pParent->Deallocate(ptr);
    }

    size_t CollectingAllocator::AllocatedSize(const void* ptr)
    {
        return m_pParent->AllocatedSize(ptr);
    }

    ezAllocatorBase::Stats CollectingAllocator::GetStats() const
    {
        return m_pParent->GetStats();
    }

}
