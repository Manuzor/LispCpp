
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
    FixedMemory::Stats::Stats() :
        m_uiAllocations(0),
        m_uiAllocatedSize(0)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    FixedMemory::FixedMemory() :
        m_uiAllocationPointer(0)
    {
    }

    EZ_FORCE_INLINE
    FixedMemory::FixedMemory(Array<byte_t> memory) :
        m_uiAllocationPointer(0),
        m_memory(memory)
    {
    }

    EZ_FORCE_INLINE
    FixedMemory::~FixedMemory()
    {
        clear();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    AllocatorResult FixedMemory::allocate(T*& out_pMemory, std::size_t uiCount)
    {
        const auto uiSize = sizeof(T) * uiCount;

        if (uiSize == 0)
        {
            out_pMemory = nullptr;
            return NothingChanged;
        }

        if (m_uiAllocationPointer + uiSize > m_memory.getSize())
        {
            out_pMemory = nullptr;
            return OutOfMemory;
        }

        out_pMemory = (T*)&m_memory[m_uiAllocationPointer];
        m_uiAllocationPointer += uiSize;

        // Update stats
        ++m_stats.m_uiAllocations;
        m_stats.m_uiAllocatedSize += uiSize;

        return Success;
    }
    
    EZ_FORCE_INLINE
    void FixedMemory::clear()
    {
        m_memory.reset();
        m_uiAllocationPointer = 0;
        m_stats = Stats();
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getAllocationPointer() const
    {
        return m_uiAllocationPointer;
    }
    
    EZ_FORCE_INLINE
    void FixedMemory::assign(Array<byte_t> memory)
    {
        m_memory = memory;
        m_uiAllocationPointer = 0;
        m_stats = Stats();
    }

    EZ_FORCE_INLINE
    void FixedMemory::operator=(Array<byte_t> memory)
    {
        assign(memory);
    }

    EZ_FORCE_INLINE
    Array<byte_t> FixedMemory::getMemory() const
    {
        return m_memory(0, m_uiAllocationPointer);
    }

    EZ_FORCE_INLINE
    Array<byte_t> FixedMemory::getEntireMemory() const
    {
        return m_memory;
    }

    EZ_FORCE_INLINE
    std::size_t FixedMemory::getAvailableMemorySize() const
    {
        return m_memory.getSize() - m_uiAllocationPointer;
    }

    EZ_FORCE_INLINE
    FixedMemory::Stats FixedMemory::getStats() const
    {
        return m_stats;
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

        EZ_ASSERT(ptr >= m_pStack->getMemory().getData(), "");
        EZ_ASSERT(ptr <= m_pStack->getMemory().getData() + m_pStack->getAllocationPointer(), "");
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
        auto stackStats = m_pStack->getStats();

        stats.m_uiNumAllocations = stackStats.m_uiAllocations;
        stats.m_uiAllocationSize = stackStats.m_uiAllocatedSize;

        return stats;
    }
}
