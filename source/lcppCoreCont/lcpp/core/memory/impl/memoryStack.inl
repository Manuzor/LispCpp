
namespace lcpp
{
    EZ_FORCE_INLINE
    MemoryStack::AllocationResult::AllocationResult() :
        m_value(None)
    {
    }

    EZ_FORCE_INLINE
    MemoryStack::AllocationResult::AllocationResult(AllocationResultEnum value) :
        m_value(value)
    {
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::isValid() const
    {
        return m_value != None;
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::nothingChanged() const
    {
        return m_value == NothingChanged;
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::succeeded() const
    {
        return m_value == Success;
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::isOutOfMemory() const
    {
        return m_value == OutOfMemory;
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::isInvalidFree() const
    {
        return m_value == InvalidFree;
    }
    
    EZ_FORCE_INLINE
    bool MemoryStack::AllocationResult::isDoubleFree() const
    {
        return m_value == DoubleFree;
    }
    
    EZ_FORCE_INLINE
    bool operator ==(const MemoryStack::AllocationResult& lhs, const MemoryStack::AllocationResult& rhs)
    {
        return lhs.m_value == rhs.m_value;
    }
    
    EZ_FORCE_INLINE
    bool operator !=(const MemoryStack::AllocationResult& lhs, const MemoryStack::AllocationResult& rhs)
    {
        return lhs.m_value != rhs.m_value;
    }

    //////////////////////////////////////////////////////////////////////////
    
    EZ_FORCE_INLINE
    MemoryStack::Stats::Stats() :
        m_uiAllocations(0),
        m_uiAllocatedSize(0)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    MemoryStack::MemoryStack() :
        m_uiAllocationPointer(0)
    {
    }

    EZ_FORCE_INLINE
    MemoryStack::MemoryStack(Ptr<ezAllocatorBase> pAllocator) :
        m_pAllocator(pAllocator),
        m_uiAllocationPointer(0)
    {
    }

    EZ_FORCE_INLINE
    MemoryStack::~MemoryStack()
    {
        clear();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    MemoryStack::AllocationResult MemoryStack::allocate(T*& out_pMemory, std::size_t uiSize)
    {
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
    void MemoryStack::clear()
    {
        if (m_memory.getSize() > 0)
        {
            m_pAllocator->Deallocate(m_memory.getData());
            m_memory.reset();
        }
    }

    EZ_FORCE_INLINE
    MemoryStack::AllocationResult MemoryStack::resize(std::size_t uiTargetSize)
    {
        EZ_ASSERT(!m_pAllocator.isNull(), "Need an allocator to allocate data.");

        if (uiTargetSize <= m_memory.getSize())
        {
            return NothingChanged;
        }

        decltype(m_memory) tempMemory;

        {
            auto pMemory = (byte_t*)m_pAllocator->Allocate(uiTargetSize, EZ_ALIGNMENT_OF(byte_t));

            if (pMemory == nullptr)
            {
                return OutOfMemory;
            }

            tempMemory.assign(pMemory, uiTargetSize);
        }

        if (m_memory.getSize() > 0)
        {
            memcpy(tempMemory.getData(), m_memory.getData(), m_uiAllocationPointer);
            memset(m_memory.getData(), Byte::Freed, m_memory.getSize());
            m_pAllocator->Deallocate(m_memory.getData());
        }

        m_memory = tempMemory;

        return Success;
    }

    EZ_FORCE_INLINE
    std::size_t MemoryStack::getAllocationPointer() const
    {
        return m_uiAllocationPointer;
    }

    EZ_FORCE_INLINE
    const Array<byte_t>& MemoryStack::getMemory() const
    {
        return m_memory;
    }

    EZ_FORCE_INLINE
    Ptr<ezAllocatorBase> MemoryStack::getAllocator()
    {
        return m_pAllocator;
    }

    EZ_FORCE_INLINE
    void MemoryStack::setAllocator(Ptr<ezAllocatorBase> pAllocator)
    {
        m_pAllocator = pAllocator;
    }

    EZ_FORCE_INLINE
    MemoryStack::Stats MemoryStack::getStats() const
    {
        return m_stats;
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    MemoryStackAllocator::MemoryStackAllocator(Ptr<MemoryStack> pWrappee) :
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
