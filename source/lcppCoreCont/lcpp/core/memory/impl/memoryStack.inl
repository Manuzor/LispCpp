
namespace lcpp
{
    EZ_FORCE_INLINE
    MemoryStack::MemoryStack(Ptr<ezAllocatorBase> pAllocator) :
        m_pAllocator(pAllocator)
    {
    }
    
    EZ_FORCE_INLINE
    MemoryStack::~MemoryStack()
    {
        if (m_memory.getSize() > 0)
        {
            // TODO Free allocated memory here.
            LCPP_NOT_IMPLEMENTED;
        }
    }
    
    EZ_FORCE_INLINE
    ezResult MemoryStack::allocate(byte_t*& out_pMemory, std::size_t uiSize)
    {
        LCPP_NOT_IMPLEMENTED;
    }
    
    EZ_FORCE_INLINE
    ezResult MemoryStack::deallocate(byte_t* pMemory)
    {
        LCPP_NOT_IMPLEMENTED;
    }

    EZ_FORCE_INLINE
    bool MemoryStack::needsResizing()
    {
        LCPP_NOT_IMPLEMENTED;
    }

    EZ_FORCE_INLINE
    void MemoryStack::resize(std::size_t uiTargetSize)
    {
        LCPP_NOT_IMPLEMENTED;
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
    MemoryStack::Stats MemoryStack::getStats() const
    {
        return m_stats;
    }
}
