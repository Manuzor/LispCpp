#pragma once
#include "lcpp/core/containers/array.h"

namespace lcpp
{
    typedef unsigned char byte_t;

    class LCPP_API_CORE_CONT MemoryStack
    {
    public:

        class Stats
        {
            std::size_t m_uiAllocations;
            std::size_t m_uiDeallocations;
            std::size_t m_uiAllocatedSize;
        };

    public:

        MemoryStack(Ptr<ezAllocatorBase> pAllocator);
        ~MemoryStack();

        ezResult allocate(byte_t*& out_pMemory, std::size_t uiSize);
        ezResult deallocate(byte_t* pMemory);

        bool needsResizing();

        void resize(std::size_t uiTargetSize);

        std::size_t getAllocationPointer() const;

        const Array<byte_t>& getMemory() const;

        Stats getStats() const;

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        /// \brief Wraps the actual data.
        Array<byte_t> m_memory;

        /// \brief Always points on the top of the stack, i.e. on the next free index.
        std::size_t m_uiAllocationPointer;

        Stats m_stats;
    };

}

#include "lcpp/core/memory/impl/memoryStack.inl"
