#pragma once
#include "lcpp/core/containers/array.h"

namespace lcpp
{
    typedef unsigned char byte_t;

    /// \brief Class that only grows.
    ///
    /// To shrink the consumed memory, you will have to
    class MemoryStack
    {
    public:

        class Stats
        {
        public:
            std::size_t m_uiAllocations;
            std::size_t m_uiDeallocations;
            std::size_t m_uiAllocatedSize;

        public:
            Stats();
        };

        enum AllocationResultEnum
        {
            Success,
            OutOfMemory,
            InvalidFree,
            DoubleFree,
        };

        class AllocationResult
        {
        public:
            AllocationResultEnum m_value;

        public:
            AllocationResult(AllocationResultEnum value);

            bool succeeded() const;
            bool isOutOfMemory() const;
            bool isInvalidFree() const;
            bool isDoubleFree() const;

        };

    public:

        MemoryStack();
        MemoryStack(Ptr<ezAllocatorBase> pAllocator);
        ~MemoryStack();

        AllocationResult allocate(byte_t*& out_pMemory, std::size_t uiSize);
        void clear();

        AllocationResult resize(std::size_t uiTargetSize);

        std::size_t getAllocationPointer() const;

        const Array<byte_t>& getMemory() const;

        Ptr<ezAllocatorBase> getAllocator();
        void setAllocator(Ptr<ezAllocatorBase> pAllocator);

        Stats getStats() const;

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        /// \brief Wraps the actual data.
        Array<byte_t> m_memory;

        /// \brief Always points on the top of the stack, i.e. on the next free index.
        std::size_t m_uiAllocationPointer;

        Stats m_stats;
    };

    bool operator ==(const MemoryStack::AllocationResult& lhs, const MemoryStack::AllocationResult& rhs);
    bool operator !=(const MemoryStack::AllocationResult& lhs, const MemoryStack::AllocationResult& rhs);

}

#include "lcpp/core/memory/impl/memoryStack.inl"
