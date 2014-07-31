
namespace lcpp
{
    template<typename T_Data>
    inline
    Ptr<LispObject>
    lcpp::LispObject::create(const MetaInfo& metaInfo)
    {
        // TODO This whole function needs more case, most likely...

        // Helper struct to determine the minimum memory needed for this lisp object using T_Data
        struct LispObjectProxy
        {
            LispObjectHeader h;
            T_Data d;
        };

        auto pAllocator = defaultAllocator();

        auto size = sizeof(LispObjectProxy);

        auto pMem = static_cast<void*>(LCPP_NEW(pAllocator, LispObjectProxy)());

        memset(pMem, 0xdadadada, size);

        return new (pMem) LispObject(metaInfo);
    }

    inline
    LispObject::LispObject(const MetaInfo& metaInfo)
    {
        m_header.m_pMetaInfo = &metaInfo;
    }

    inline
    bool LispObject::isType(const Type& type) const
    {
        return getType() == type;
    }

    inline
    const Type& LispObject::getType() const
    {
        EZ_ASSERT(m_header.m_pMetaInfo, "No valid type info.");
        return m_header.m_pMetaInfo->getType();
    }

    inline
    const MetaInfo& LispObject::getMetaInfo() const
    {
        EZ_ASSERT(m_header.m_pMetaInfo, "No valid type info.");
        return *m_header.m_pMetaInfo;
    }

    inline
    const LispObjectHeader& LispObject::getHeader() const
    {
        return m_header;
    }
}
