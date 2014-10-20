#include "stdafx.h"
#include "lcpp/foundation/memory/allocator.h"

ezAllocatorBase* lcpp::defaultAllocator()
{
    static NonTrackingHeapAllocator allocator("lcpp/NonTrackingHeapAllocator");
    static ElectricFenceAllocator ef(&allocator);

    return &ef;
}

lcpp::ElectricFenceAllocator::ElectricFenceAllocator(ezAllocatorBase* pInternalContainerAllocator) :
    m_freeQueue(pInternalContainerAllocator),
    m_allocationMap(ezCompareHelper<void*>(), pInternalContainerAllocator)
{
}

void* lcpp::ElectricFenceAllocator::Allocate(size_t uiSize, size_t uiAlign)
{
    auto numPages = (uiSize - 1) / s_pageSize + 2;
    auto sizeNeeded = numPages * s_pageSize;

    auto address = static_cast<char*>(VirtualAlloc(
        nullptr, // optional starting address
        sizeNeeded,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
        ));

    if (address == nullptr)
    {
        wchar_t buffer[2048];
        DWORD errorCode = GetLastError();
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr,
            errorCode,
            0,
            (LPTSTR)buffer,
            2048,
            nullptr);
        EZ_ASSERT(false, "Error allocating memory!");
    }

    auto padding = s_pageSize - (uiSize % s_pageSize);
    if (uiSize % s_pageSize == 0)
    {
        padding = 0;
    }
    else
    {
        padding &= ~0x7;
    }

    EZ_ASSERT(padding % 8 == 0, "");
    auto result = address + padding;

    DWORD unused;
    VirtualProtect(
        address + sizeNeeded - s_pageSize,
        s_pageSize,
        PAGE_NOACCESS,
        &unused);
    *result = 0;
    *(address + sizeNeeded - s_pageSize - 1) = 0;

    AllocationInfo allocationInfo = { address, sizeNeeded };
    m_allocationMap[result] = allocationInfo;

    return result;
}

void lcpp::ElectricFenceAllocator::Deallocate(void* ptr)
{
    if (ptr == nullptr)
        return;

    auto it = m_allocationMap.Find(ptr);
    EZ_ASSERT(it.IsValid(), "Double or invalid free!");

    auto allocationInfo = it.Value();
    m_allocationMap.Erase(it);

    DWORD unused;
    VirtualProtect(
        allocationInfo.ptr,
        allocationInfo.size,
        PAGE_NOACCESS,
        &unused
        );

    m_freeQueue.PushBack(allocationInfo);
    if (m_freeQueue.GetCount() > s_maxFreeQueueElements)
    {
        auto allocationInfo = m_freeQueue.PeekFront();
        m_freeQueue.PopFront();
        VirtualFree(allocationInfo.ptr, allocationInfo.size, MEM_DECOMMIT | MEM_RELEASE);
    }
}

size_t lcpp::ElectricFenceAllocator::AllocatedSize(const void* ptr)
{
    auto it = m_allocationMap.Find(const_cast<void*>(ptr));
    EZ_ASSERT(it.IsValid(), "Double or invalid free!");
    return it.Value().size;
}

ezAllocatorBase::Stats lcpp::ElectricFenceAllocator::GetStats() const
{
    return Stats();
}
