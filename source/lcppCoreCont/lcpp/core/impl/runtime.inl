
namespace lcpp
{

    EZ_FORCE_INLINE
    Ptr<ezAllocatorBase> LispRuntime::allocator()
    {
        return m_pAllocator;
    }

    EZ_FORCE_INLINE
    Ptr<const LispObject> LispRuntime::getSyntaxEnvironment() const
    {
        return m_pSyntaxEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<LispObject> LispRuntime::getSyntaxEnvironment()
    {
        return m_pSyntaxEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<const LispObject> LispRuntime::getGlobalEnvironment() const
    {
        return m_pGlobalEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<LispObject> LispRuntime::getGlobalEnvironment()
    {
        return m_pGlobalEnvironment;
    }

    EZ_FORCE_INLINE
    Ptr<reader::State> LispRuntime::getReaderState()
    {
        return m_pReaderState;
    }

    EZ_FORCE_INLINE
    Ptr<const reader::State> LispRuntime::getReaderState() const
    {
        return m_pReaderState;
    }

    EZ_FORCE_INLINE
    ezUInt32 LispRuntime::getRecursionDepth() const
    {
        return m_recursionDepth;
    }

    EZ_FORCE_INLINE
    ezUInt32 LispRuntime::getRecursionLimit() const
    {
        return m_recursionLimit;
    }

}
