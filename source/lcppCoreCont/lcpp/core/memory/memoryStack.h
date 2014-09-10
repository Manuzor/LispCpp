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
        MemoryStack(Array<byte_t> memory);
        ~MemoryStack();

        /// \brief Allocates enough bytes to store a \a T object in it.
        /// \param out_pMemory The adress of the allocated memory.
        /// \param uiCount The number of \a T instances to allocate. Defaults to 1.
        template<typename T>
        AllocationResult allocate(T*& out_pMemory, std::size_t uiCount = 1);

        void clear();

        /// \brief
        void assign(Array<byte_t> memory);

        /// \see assign
        void operator =(Array<byte_t> memory);

        /// \brief Gets the part of the available memory that contains valid allocations.
        Array<byte_t> getMemory() const;

        /// \brief Gets the entire memory block used by this MemoryStack.
        /// \remark If you need
        Array<byte_t> getEntireMemory() const;

        /// \brief The current index of the next free byte within this memory.
        /// Will always be the same as getMemory().getSize().
        std::size_t getAllocationPointer() const;

        std::size_t getAvailableMemorySize() const;

        Stats getStats() const;

    private:

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
