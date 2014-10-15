
namespace lcpp
{
    EZ_FORCE_INLINE
    AllocatorResult::AllocatorResult() :
        m_value(None)
    {
    }

    EZ_FORCE_INLINE
    AllocatorResult::AllocatorResult(AllocatorResultEnum value) :
        m_value(value)
    {
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::isValid() const
    {
        return m_value != None;
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::nothingChanged() const
    {
        return m_value == NothingChanged;
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::succeeded() const
    {
        return m_value == Success;
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::isOutOfMemory() const
    {
        return m_value == OutOfMemory;
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::isInvalidFree() const
    {
        return m_value == InvalidFree;
    }

    EZ_FORCE_INLINE
    bool AllocatorResult::isDoubleFree() const
    {
        return m_value == DoubleFree;
    }

    EZ_FORCE_INLINE
    bool operator ==(const AllocatorResult& lhs, const AllocatorResult& rhs)
    {
        return lhs.m_value == rhs.m_value;
    }

    EZ_FORCE_INLINE
    bool operator !=(const AllocatorResult& lhs, const AllocatorResult& rhs)
    {
        return lhs.m_value != rhs.m_value;
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    FixedMemory::FixedMemory()
    {
        ezMemoryUtils::ZeroFill(this);
    }

    EZ_FORCE_INLINE
    FixedMemory::~FixedMemory()
    {
        free();
    }

    template<typename T>
    EZ_FORCE_INLINE
    AllocatorResult FixedMemory::allocate(T*& out_pMemory, std::size_t uiCount)
    {
        EZ_ASSERT(m_pBegin, "Invalid state.");

        const auto uiSize = sizeof(T) * uiCount;

        if (uiSize == 0)
        {
            out_pMemory = nullptr;
            return NothingChanged;
        }

        if (m_pAllocationPointer + uiSize > m_pEnd)
        {
            out_pMemory = nullptr;
            return OutOfMemory;
        }

        out_pMemory = (T*)m_pAllocationPointer;
        m_pAllocationPointer += uiSize;

        LCPP_InDebug( memset(out_pMemory, 0xcacacaca, uiSize); );

        // Update stats
        ++m_uiNumAllocations;

        return Success;
    }

    EZ_FORCE_INLINE
    void FixedMemory::initialize(size_t uiNumPages)
    {
        const size_t uiMemorySize = uiNumPages * GarbageCollectorPageSize;
        m_pBegin = (byte_t*)VirtualAlloc(nullptr, uiMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        EZ_ASSERT(m_pBegin != nullptr, "");
        m_pEnd = m_pBegin + uiMemorySize;
        internalReset();

        LCPP_InDebug( fill(0xcdcdcdcd); );
    }

    EZ_FORCE_INLINE
    void FixedMemory::reset()
    {
        EZ_ASSERT(m_pBegin, "Cannot reset after free was called. You have to call initialize again.");

        removeProtection();
        internalReset();

        LCPP_InDebug( fill(0xcfcfcfcf); );
    }

    EZ_FORCE_INLINE
    void FixedMemory::internalReset()
    {
        m_pAllocationPointer = m_pBegin;
        m_uiNumAllocations = 0;
        LCPP_InDebug( m_state = State::Available; );
    }

    EZ_FORCE_INLINE
    void FixedMemory::protect()
    {
        DWORD oldProtect;
        LCPP_InDebug( fill(0xbaadf00d); );
        bool result = VirtualProtect(m_pBegin, getEntireMemorySize(), PAGE_NOACCESS, &oldProtect) == 0 ? false : true;
        EZ_ASSERT(result, "VirtualProtect PAGE_NOACCESS failed.");
        LCPP_InDebug( m_state = State::Protected; );
    }

    EZ_FORCE_INLINE
    void FixedMemory::removeProtection()
    {
        DWORD oldProtect;
        bool result = VirtualProtect(m_pBegin, getEntireMemorySize(), PAGE_READWRITE, &oldProtect) == 0 ? false : true;
        EZ_ASSERT(result, "VirtualProtect PAGE_READWRITE failed.");
        LCPP_InDebug( m_state = State::Unprotected; );
    }

    EZ_FORCE_INLINE
    void FixedMemory::free()
    {
        if (m_pBegin == nullptr)
            return;

        LCPP_InDebug
        (
            removeProtection();
            fill(0xfeeefeee);
        );

        VirtualFree(m_pBegin, getEntireMemorySize(), MEM_DECOMMIT | MEM_RELEASE);
        m_pBegin = nullptr;
        m_pAllocationPointer = nullptr;
        m_pEnd = nullptr;
        m_uiNumAllocations = 0;

        LCPP_InDebug( m_state = State::Freed; );
    }

    EZ_FORCE_INLINE
    const lcpp::byte_t* FixedMemory::getBeginning() const
    {
        return m_pBegin;
    }

    EZ_FORCE_INLINE
    lcpp::byte_t* FixedMemory::getBeginning()
    {
        return m_pBegin;
    }

    EZ_FORCE_INLINE
    const lcpp::byte_t* FixedMemory::getEnd() const
    {
        return m_pEnd;
    }

    EZ_FORCE_INLINE
    lcpp::byte_t* FixedMemory::getEnd()
    {
        return m_pEnd;
    }

    EZ_FORCE_INLINE
    const lcpp::byte_t* FixedMemory::getAllocationPointer() const
    {
        return m_pAllocationPointer;
    }

    EZ_FORCE_INLINE
    lcpp::byte_t* FixedMemory::getAllocationPointer()
    {
        return m_pAllocationPointer;
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getEntireMemorySize() const
    {
        return m_pEnd - m_pBegin;
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getAllocatedMemorySize() const
    {
        return m_pAllocationPointer - m_pBegin;
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getFreeMemorySize() const
    {
        return m_pEnd - m_pAllocationPointer;
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getNumAllocations() const
    {
        return m_uiNumAllocations;
    }

    EZ_FORCE_INLINE
    bool FixedMemory::contains(byte_t* ptr) const
    {
        return ptr >= m_pBegin && ptr < m_pEnd;
    }

    EZ_FORCE_INLINE
    void FixedMemory::fill(int pattern)
    {
        memset(m_pBegin, pattern, getEntireMemorySize());
    }

    EZ_FORCE_INLINE
    float FixedMemory::getPercentageFilled() const
    {
        auto entireSize = double(getEntireMemorySize());
        auto allocatedSize = double(getAllocatedMemorySize());
        return float(allocatedSize / entireSize);
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    MemoryStackAllocator::MemoryStackAllocator(Ptr<FixedMemory> pWrappee) :
        m_pStack(pWrappee)
    {
    }

    EZ_FORCE_INLINE
    void* MemoryStackAllocator::Allocate(size_t uiSize, size_t uiAlign)
    {
        EZ_ASSERT(!m_pStack.isNull(), "");

        byte_t* pMemory(nullptr);
        auto result = m_pStack->allocate(pMemory, uiSize);
        EZ_CHECK_ALIGNMENT(pMemory, uiAlign);

        EZ_ASSERT(result.succeeded(), "");

        return pMemory;
    }

    EZ_FORCE_INLINE
    void MemoryStackAllocator::Deallocate(void* ptr)
    {
        EZ_ASSERT(!m_pStack.isNull(), "");
        EZ_ASSERT(m_pStack->contains((byte_t*)ptr), "");
    }

    EZ_FORCE_INLINE
    size_t MemoryStackAllocator::AllocatedSize(const void* ptr)
    {
        EZ_ASSERT(!m_pStack.isNull(), "");
        return 0;
    }

    EZ_FORCE_INLINE
    ezAllocatorBase::Stats MemoryStackAllocator::GetStats() const
    {
        EZ_ASSERT(!m_pStack.isNull(), "");

        Stats stats;
        stats.m_uiNumAllocations = m_pStack->getNumAllocations();
        stats.m_uiAllocationSize = m_pStack->getAllocatedMemorySize();

        return stats;
    }
}
