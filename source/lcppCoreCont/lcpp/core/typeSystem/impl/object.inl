
namespace lcpp
{
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

}
