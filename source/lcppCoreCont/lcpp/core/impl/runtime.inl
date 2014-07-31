
namespace lcpp
{

    EZ_FORCE_INLINE
    Ptr<ezAllocatorBase> LispRuntimeState::allocator()
    {
        return m_pAllocator;
    }

    EZ_FORCE_INLINE
    Ptr<const LispObject> LispRuntimeState::getSyntaxEnvironment() const
    {
        return m_pSyntaxEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<LispObject> LispRuntimeState::getSyntaxEnvironment()
    {
        return m_pSyntaxEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<const LispObject> LispRuntimeState::getGlobalEnvironment() const
    {
        return m_pGlobalEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<LispObject> LispRuntimeState::getGlobalEnvironment()
    {
        return m_pGlobalEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<reader::State> LispRuntimeState::getReaderState()
    {
        return m_pReaderState;
    }

    EZ_FORCE_INLINE
    Ptr<const reader::State> LispRuntimeState::getReaderState() const
    {
        return m_pReaderState;
    }

    EZ_FORCE_INLINE
    ezUInt32 LispRuntimeState::getRecursionDepth() const
    {
        return m_recursionDepth;
    }

    EZ_FORCE_INLINE
    ezUInt32 LispRuntimeState::getRecursionLimit() const
    {
        return m_recursionLimit;
    }

}
