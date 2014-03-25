#pragma once

void* operator new(size_t size)
{
    return ezFoundation::GetDefaultAllocator()->Allocate(size, EZ_ALIGNMENT_MINIMUM);
}

void* operator new[](size_t size)
{
    return ezFoundation::GetDefaultAllocator()->Allocate(size, EZ_ALIGNMENT_MINIMUM);
}

void operator delete(void* mem) throw()
{
    ezFoundation::GetDefaultAllocator()->Deallocate(mem);
}

void operator delete[](void* mem)
{
    ezFoundation::GetDefaultAllocator()->Deallocate(mem);
}
