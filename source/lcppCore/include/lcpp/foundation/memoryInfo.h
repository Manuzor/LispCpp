#pragma once

namespace lcpp
{
    struct MemoryInfo
    {
        size_t size;
        size_t alignment;

        MemoryInfo(size_t size, size_t alignment) :
            size(size),
            alignment(alignment)
        {
        }
    };
}
