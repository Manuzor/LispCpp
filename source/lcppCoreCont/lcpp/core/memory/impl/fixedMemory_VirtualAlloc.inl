
namespace lcpp
{
    EZ_FORCE_INLINE
    void FixedMemory::initialize(size_t uiNumPages)
    {
        const size_t uiMemorySize = uiNumPages * GarbageCollectorPageSize;
        m_pBegin = (byte_t*)VirtualAlloc(nullptr, uiMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (m_pBegin == nullptr)
        {
            wchar_t buffer[2048];
            DWORD errorCode = GetLastError();
            FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                errorCode,
                0,
                (LPTSTR)buffer,
                2048,
                nullptr);
            EZ_REPORT_FAILURE("VirtualAlloc failed.");
        }

        m_pEnd = m_pBegin + uiMemorySize;
        internalReset();

        LCPP_InDebug( fill(0xcdcdcdcd); );
    }

    EZ_FORCE_INLINE
    void FixedMemory::protect()
    {
        DWORD oldProtect;
        LCPP_InDebug( fill(0xbaadf00d); );
        bool result = VirtualProtect(m_pBegin, getEntireMemorySize(), PAGE_NOACCESS, &oldProtect) == 0 ? false : true;
        if (!result)
        {
            wchar_t buffer[2048];
            DWORD errorCode = GetLastError();
            FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                errorCode,
                0,
                (LPTSTR)buffer,
                2048,
                nullptr);
            EZ_REPORT_FAILURE("VirtualProtect with PAGE_NOACCESS failed.");
        }

        LCPP_InDebug( m_state = State::Protected; );
    }

    EZ_FORCE_INLINE
    void FixedMemory::removeProtection()
    {
        DWORD oldProtect;
        bool result = VirtualProtect(m_pBegin, getEntireMemorySize(), PAGE_READWRITE, &oldProtect) == 0 ? false : true;
        if (!result)
        {
            wchar_t buffer[2048];
            DWORD errorCode = GetLastError();
            FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                errorCode,
                0,
                (LPTSTR)buffer,
                2048,
                nullptr);
            EZ_REPORT_FAILURE("VirtualProtect with PAGE_READWRITE failed.");
        }
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

        VirtualFree(m_pBegin, getEntireMemorySize(), MEM_DECOMMIT | MEM_RELEASE);
        m_pBegin = nullptr;
        m_pAllocationPointer = nullptr;
        m_pEnd = nullptr;
        m_uiNumAllocations = 0;

        LCPP_InDebug( m_state = State::Freed; );
    }
}
