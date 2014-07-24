#pragma once

namespace lcpp
{
    struct MemoryInfo
    {
        size_t size;
        size_t alignment;

        template<typename T>
        static MemoryInfo create()
        {
            return MemoryInfo(sizeof(T), EZ_ALIGNMENT_OF(T));
        }

        template<typename T>
        static MemoryInfo createFromAbstractType()
        {
            return MemoryInfo(sizeof(T), sizeof(T));
        }

        MemoryInfo(size_t size, size_t alignment) :
            size(size),
            alignment(alignment)
        {
        }
    };
}
