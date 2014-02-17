#pragma once

#include <Foundation/Math/Math.h>
#include <Foundation/Memory/AllocatorBase.h>
#include <Foundation/Memory/MemoryTracker.h>
#include <Foundation/Threading/ThreadUtils.h>

/// \brief Policy based allocator implementation of the ezAllocatorBase interface.
///
/// AllocationPolicy defines how the actual memory is allocated.\n
/// TrackingFlags defines how stats about allocations are tracked.\n
template <typename AllocationPolicy, ezUInt32 TrackingFlags = ezMemoryTrackingFlags::Default>
class ezAllocator : public ezAllocatorBase
{
public:
  ezAllocator(const char* szName, ezAllocatorBase* pParent = NULL);
  ~ezAllocator();

  // ezAllocatorBase implementation
  virtual void* Allocate(size_t uiSize, size_t uiAlign) EZ_OVERRIDE;
  virtual void Deallocate(void* ptr) EZ_OVERRIDE;
  virtual size_t AllocatedSize(const void* ptr) EZ_OVERRIDE;
  virtual Stats GetStats() const EZ_OVERRIDE;

  ezAllocatorBase* GetParent() const;

private:
  AllocationPolicy m_allocator;
  
  ezAllocatorId m_Id;
  ezThreadHandle m_threadHandle;
};

#include <Foundation/Memory/Implementation/Allocator_inl.h>

