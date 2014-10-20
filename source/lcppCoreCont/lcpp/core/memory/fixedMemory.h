#pragma once
#include "lcpp/core/memory/garbageCollectionCommon.h"
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

    enum AllocatorResultEnum
    {
        None,

        NothingChanged,
        Success,
        OutOfMemory,
        InvalidFree,
        DoubleFree,
    };

    class AllocatorResult
    {
    public:
        AllocatorResultEnum m_value;

    public:
        AllocatorResult();
        AllocatorResult(AllocatorResultEnum value);

        bool isValid() const;

        bool nothingChanged() const;
        bool succeeded() const;
        bool isOutOfMemory() const;
        bool isInvalidFree() const;
        bool isDoubleFree() const;

    };

    /// \brief Class that only grows.
    ///
    /// To shrink the consumed memory, you will have to
    class FixedMemory
    {
    public:

        enum class State
        {
            Invalid,
            Available,
            Freed,
            Protected,
            Unprotected,
        };

    public:
        FixedMemory(FixedMemory&&) = delete;
        FixedMemory(const FixedMemory&) = delete;
        void operator=(const FixedMemory&) = delete;

        FixedMemory();
        ~FixedMemory();

        /// \brief Allocates enough bytes to store a \a T object in it.
        /// \param out_pMemory The adress of the allocated memory.
        /// \param uiCount The number of \a T instances to allocate. Defaults to 1.
        template<typename T>
        AllocatorResult allocate(T*& out_pMemory, std::size_t uiCount = 1);

        /// \brief Will allocate new memory without touching the previously allocated memory.
        /// \remarks Memory will be leaked if \c free was not called before and this is not the initial call to this function.
        void initialize(size_t uiNumPages);

        /// \brief Resets the state of this instance without allocating or freeing memory.
        void reset();

        /// \brief Protects the entire allocated memory from any kind of access (read/write/execute).
        void protect();

        /// \brief Frees the allocated memory and effectively invalidates this object.
        void free();

        const byte_t* getBeginning() const;         ///< Points to the first valid byte.
        byte_t* getBeginning();                     ///< Points to the first valid byte.

        const byte_t* getEnd() const;               ///< Points past the first valid byte.
        byte_t* getEnd();                           ///< Points past the first valid byte.

        const byte_t* getAllocationPointer() const; ///< Points to the first byte that would be allocated by \a allocate.
        byte_t* getAllocationPointer();             ///< Points to the first byte that would be allocated by \a allocate.

        std::size_t getAllocatedMemorySize() const;
        std::size_t getFreeMemorySize() const;
        std::size_t getEntireMemorySize() const;
        std::size_t getNumAllocations() const;
        float getPercentageFilled() const;

        bool contains(byte_t* ptr) const;

    private:

        void internalReset();
        void fill(int pattern);
        void removeProtection();

        byte_t* m_pBegin;
        byte_t* m_pEnd;
        byte_t* m_pAllocationPointer;

        ezUInt64 m_uiNumAllocations;

        State m_state;
    };

    bool operator ==(const AllocatorResult& lhs, const AllocatorResult& rhs);
    bool operator !=(const AllocatorResult& lhs, const AllocatorResult& rhs);

    class MemoryStackAllocator :
        public ezAllocatorBase
    {
    public:

        MemoryStackAllocator(Ptr<FixedMemory> pWrappee);

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;

        /// \brief Checks wether the ptr is a valid ptr to deallocate.
        /// \remark The wrapped MemoryStack does not support direct deallocations.
        ///         This method simply checks whether the given \a ptr is valid or not.
        virtual void Deallocate(void* ptr) override;

        virtual size_t AllocatedSize(const void* ptr) override;

        virtual Stats GetStats() const override;

    private:
        Ptr<FixedMemory> m_pStack;
    };
}

#include "lcpp/core/memory/impl/fixedMemory.inl"
