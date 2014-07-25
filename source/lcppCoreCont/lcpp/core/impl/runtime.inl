
namespace lcpp
{

    inline
    Ptr<ezAllocatorBase>
    LispRuntime::allocator()
    {
        return m_pAllocator;
    }

    inline
    void
    LispRuntime::allocator(Ptr<ezAllocatorBase> value)
    {
        m_pAllocator = value;
    }

    inline
    Ptr<const LispObject>
    LispRuntime::syntaxEnvironment() const
    {
        return m_pSyntaxEnvironment;
    }

    inline
    Ptr<LispObject>
    LispRuntime::syntaxEnvironment()
    {
        return m_pSyntaxEnvironment;
    }

    inline
    void
    LispRuntime::syntaxEnvironment(Ptr<LispObject> value)
    {
        m_pSyntaxEnvironment = value;
    }

    inline
    Ptr<const LispObject>
    LispRuntime::globalEnvironment() const
    {
        return m_pGlobalEnvironment;
    }

    inline
    Ptr<LispObject>
    LispRuntime::globalEnvironment()
    {
        return m_pGlobalEnvironment;
    }

    inline
    void
    LispRuntime::globalEnvironment(Ptr<LispObject> value)
    {
        m_pGlobalEnvironment = value;
    }

    inline
    const LispRuntime::InstanceTables&
    LispRuntime::instanceTables() const
    {
        return m_instanceTables;
    }

    inline
    ezUInt32
    LispRuntime::recursionDepth() const
    {
        return m_recursionDepth;
    }

    inline
    ezUInt32
    LispRuntime::recursionLimit() const
    {
        return m_recursionLimit;
    }

}
