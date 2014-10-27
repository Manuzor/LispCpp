
namespace lcpp
{

    EZ_FORCE_INLINE
    Ptr<GarbageCollector> LispRuntimeState::getGarabgeCollector()
    {
        return m_pGC;
    }

    EZ_FORCE_INLINE
    Ptr<LispObject> LispRuntimeState::getSyntaxEnvironment()
    {
        return m_pSyntaxEnvironment;
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
    Ptr<printer::State> LispRuntimeState::getPrinterState()
    {
        return m_pPrinterState;
    }

    EZ_FORCE_INLINE
    Ptr<const printer::State> LispRuntimeState::getPrinterState() const
    {
        return m_pPrinterState;
    }

    EZ_FORCE_INLINE
    void LispRuntimeState::setBaseDirectory(const char* szName)
    {
        m_baseDirectory = szName;
    }

    EZ_FORCE_INLINE
        const char* LispRuntimeState::getBaseDirectory() const
    {
        return m_baseDirectory.GetData();
    }
    EZ_FORCE_INLINE
    void LispRuntimeState::setUserDirectory(const char* szName)
    {
        m_userDirectory = szName;
    }

    EZ_FORCE_INLINE
    const char* LispRuntimeState::getUserDirectory() const
    {
        return m_userDirectory.GetData();
    }

}
