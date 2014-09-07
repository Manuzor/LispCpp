#pragma once
#include "lcpp/core/containers/array.h"

namespace lcpp
{
    typedef unsigned char byte_t;

    class Byte
    {
    public:

        EZ_DECLARE_POD_TYPE();

        enum Enum : unsigned char
        {
            Unallocated = 0xfa,
            Allocated = 0xfb,
            Freed = 0xfc,
            Protected = 0xfd,
        };

    public:

        Byte(unsigned char value) :
            m_value((Enum)value)
        {
        }

        Byte(Enum value) :
            m_value(value)
        {
        }

    private:
        Enum m_value;
    };

    EZ_CHECK_AT_COMPILETIME(sizeof(Byte) == sizeof(unsigned char));

    /// \brief Class that only grows.
    ///
    /// To shrink the consumed memory, you will have to
    class MemoryStack
    {
    public:

        class Stats
        {
        public:
            EZ_DECLARE_POD_TYPE();

            std::size_t m_uiAllocations;
            std::size_t m_uiAllocatedSize;

        public:
            Stats();
        };

        enum AllocationResultEnum
        {
            None,

            NothingChanged,
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
            AllocationResult();
            AllocationResult(AllocationResultEnum value);

            bool isValid() const;

            bool nothingChanged() const;
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

    class MemoryStackAllocator :
        public ezAllocatorBase
    {
    public:

        MemoryStackAllocator(Ptr<MemoryStack> pWrappee);

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;

        /// \brief Checks wether the ptr is a valid ptr to deallocate.
        /// \remark The wrapped MemoryStack does not support direct deallocations.
        ///         This method simply checks whether the given \a ptr is valid or not.
        virtual void Deallocate(void* ptr) override;

        virtual size_t AllocatedSize(const void* ptr) override;

        virtual Stats GetStats() const override;

    private:
        Ptr<MemoryStack> m_pStack;
    };
}

#include "lcpp/core/memory/impl/memoryStack.inl"
