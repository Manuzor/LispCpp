#include <Foundation/PCH.h>
#include <Foundation/Memory/CommonAllocators.h>
#include <Foundation/Communication/GlobalEvent.h>

enum 
{ 
  HEAP_ALLOCATOR_BUFFER_SIZE = sizeof(ezHeapAllocator),
  ALIGNED_ALLOCATOR_BUFFER_SIZE = sizeof(ezAlignedHeapAllocator)
};

static ezUInt8 s_DefaultAllocatorBuffer[HEAP_ALLOCATOR_BUFFER_SIZE];
static ezUInt8 s_StaticAllocatorBuffer[HEAP_ALLOCATOR_BUFFER_SIZE];

static ezUInt8 s_AlignedAllocatorBuffer[ALIGNED_ALLOCATOR_BUFFER_SIZE];

bool ezFoundation::s_bIsInitialized = false;
ezAllocatorBase* ezFoundation::s_pDefaultAllocator = NULL;
ezAllocatorBase* ezFoundation::s_pAlignedAllocator = NULL;

void ezFoundation::Initialize()
{
  if (s_bIsInitialized)
    return;

#if EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
  ezMemoryUtils::ReserveLower4GBAddressSpace();
#endif
   
  if (s_pDefaultAllocator == NULL)
  {
    s_pDefaultAllocator = new (s_DefaultAllocatorBuffer) ezHeapAllocator("DefaultHeap");
  }

  if (s_pAlignedAllocator == NULL)
  {
    s_pAlignedAllocator = new (s_AlignedAllocatorBuffer) ezAlignedHeapAllocator("AlignedHeap");
  }

  s_bIsInitialized = true;
}

void ezFoundation::Shutdown()
{
  if (!s_bIsInitialized)
    return;

  ezGlobalEvent::Broadcast("ezFoundation_Shutdown");

  // Allocators must not be deleted, they might still be used during application shutdown
  // but dump memory leaks instead
  ezMemoryTracker::DumpMemoryLeaks();

  s_bIsInitialized = false;
}

#if defined(EZ_CUSTOM_STATIC_ALLOCATOR_FUNC)
  extern ezAllocatorBase* EZ_CUSTOM_STATIC_ALLOCATOR_FUNC();
#endif

ezAllocatorBase* ezFoundation::GetStaticAllocator()
{
  static ezAllocatorBase* pStaticAllocator = NULL;

  if (pStaticAllocator == NULL)
  {
#if defined(EZ_CUSTOM_STATIC_ALLOCATOR_FUNC)

#if EZ_ENABLED(EZ_COMPILE_ENGINE_AS_DLL)
    
  #if EZ_ENABLED(EZ_PLATFORM_WINDOWS)
    typedef ezAllocatorBase* (*GetStaticAllocatorFunc)();

    HMODULE hThisModule = GetModuleHandle(NULL);
    GetStaticAllocatorFunc func = (GetStaticAllocatorFunc)GetProcAddress(hThisModule, EZ_CUSTOM_STATIC_ALLOCATOR_FUNC);
    if (func != NULL)
    {
      pStaticAllocator = (*func)();
      return pStaticAllocator;
    }
  #else
    #error "Customizing static allocator not implemented"
  #endif

#else
    return EZ_CUSTOM_STATIC_ALLOCATOR_FUNC();
#endif

#endif

    pStaticAllocator = new (s_StaticAllocatorBuffer) ezHeapAllocator(EZ_STATIC_ALLOCATOR_NAME);
  }

  return pStaticAllocator;
}




EZ_STATICLINK_FILE(Foundation, Foundation_Basics_Basics);

