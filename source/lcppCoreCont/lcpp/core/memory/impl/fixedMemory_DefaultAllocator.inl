
namespace lcpp
{
    EZ_FORCE_INLINE
    void FixedMemory::initialize(size_t uiNumPages)
    {
        const size_t uiMemorySize = uiNumPages * GarbageCollectorPageSize;
        m_pBegin = (byte_t*)defaultAllocator()->Allocate(uiMemorySize, 2);

        EZ_ASSERT(m_pBegin != nullptr, "Initializing fixed memory pool failed.");

        m_pEnd = m_pBegin + uiMemorySize;
        internalReset();

        LCPP_InDebug( fill(0xcdcdcdcd); );
    }

    EZ_FORCE_INLINE
    void FixedMemory::protect()
    {
        // Note: This function basically doesn't do anything for this version of the FixedMemory implementation.
        LCPP_InDebug( fill(0xbaadf00d); );
        LCPP_InDebug( m_state = State::Protected; );
    }

    EZ_FORCE_INLINE
    void FixedMemory::removeProtection()
    {
        // Note: This function basically doesn't do anything for this version of the FixedMemory implementation.
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

        defaultAllocator()->Deallocate(m_pBegin);
        m_pBegin = nullptr;
        m_pAllocationPointer = nullptr;
        m_pEnd = nullptr;
        m_uiNumAllocations = 0;

        LCPP_InDebug( m_state = State::Freed; );
    }
}
