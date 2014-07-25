
namespace lcpp
{
    inline
    LispObject::LispObject(const MetaInfo& metaInfo) :
        m_pMetaInfo(&metaInfo)
    {
    }

    inline
    LispObject::~LispObject()
    {
        m_pMetaInfo = nullptr;
    }

    inline
    bool LispObject::isType(const Type& type) const
    {
        return getType() == type;
    }

    inline
    const Type& LispObject::getType() const
    {
        EZ_ASSERT(m_pMetaInfo, "No valid type info.");
        return m_pMetaInfo->getType();
    }

    inline
    const MetaInfo& LispObject::getMetaInfo() const
    {
        EZ_ASSERT(m_pMetaInfo, "No valid type info.");
        return *m_pMetaInfo;
    }

}
