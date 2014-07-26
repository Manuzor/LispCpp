
namespace lcpp
{
    template<typename T_Data>
    inline
    Ptr<LispObject>
    lcpp::LispObject::create(const MetaInfo& metaInfo)
    {
        // TODO This whole function needs more case, most likely...

        auto pAllocator = LispRuntime::instance()->allocator();

        auto sizeA = sizeof(LispObjectHeader);
        auto alignmentA = EZ_ALIGNMENT_OF(LispObjectHeader);

        auto sizeB = sizeof(T_Data);
        auto alignmentB = EZ_ALIGNMENT_OF(T_Data);

        auto allocationSize = sizeA + sizeB;
        auto alignment = ezMath::Max(alignmentA, alignmentB);

        auto pMem = (char*)pAllocator->Allocate(allocationSize, alignment);

        return new(pMem)LispObject(metaInfo);
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

    inline
    const LispObjectBody& LispObject::getBody() const
    {
        return m_body;
    }

    inline
    LispObjectBody& LispObject::getBody()
    {
        return m_body;
    }
}
