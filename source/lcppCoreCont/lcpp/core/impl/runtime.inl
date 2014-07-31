
namespace lcpp
{

    inline
    Ptr<ezAllocatorBase> LispRuntime::allocator()
    {
        return m_pAllocator;
    }

    inline
    void LispRuntime::allocator(Ptr<ezAllocatorBase> value)
    {
        m_pAllocator = value;
    }

    inline
    Ptr<const LispObject> LispRuntime::syntaxEnvironment() const
    {
        return m_pSyntaxEnvironment;
    }

    inline
    Ptr<LispObject> LispRuntime::syntaxEnvironment()
    {
        return m_pSyntaxEnvironment;
    }

    inline
    void LispRuntime::syntaxEnvironment(Ptr<LispObject> value)
    {
        m_pSyntaxEnvironment = value;
    }

    inline
    Ptr<const LispObject> LispRuntime::globalEnvironment() const
    {
        return m_pGlobalEnvironment;
    }

    inline
    Ptr<LispObject> LispRuntime::globalEnvironment()
    {
        return m_pGlobalEnvironment;
    }

    inline
    void LispRuntime::globalEnvironment(Ptr<LispObject> value)
    {
        m_pGlobalEnvironment = value;
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

    inline
    ezUInt32 LispRuntime::recursionDepth() const
    {
        return m_recursionDepth;
    }

    inline
    ezUInt32 LispRuntime::recursionLimit() const
    {
        return m_recursionLimit;
    }

}
