
namespace lcpp
{
    EZ_FORCE_INLINE
    MemoryStack::AllocationResult::AllocationResult(AllocationResultEnum value) :
        m_value(value)
    {
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
        m_uiDeallocations(0),
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
    
    EZ_FORCE_INLINE
    MemoryStack::AllocationResult MemoryStack::allocate(byte_t*& out_pMemory, std::size_t uiSize)
    {
        if (uiSize == 0)
        {
            out_pMemory = nullptr;
            return Success;
        }

        if (m_uiAllocationPointer + uiSize > m_memory.getSize())
        {
            out_pMemory = nullptr;
            return OutOfMemory;
        }

        out_pMemory = &m_memory[m_uiAllocationPointer];
        m_uiAllocationPointer += uiSize;

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
        EZ_ASSERT(uiTargetSize >= m_memory.getSize(), "MemoryStack cannot shrink, it can only grow!");

        if (uiTargetSize == m_memory.getSize())
        {
            return Success;
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
}