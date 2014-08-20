
namespace lcpp
{
    EZ_FORCE_INLINE
    LispObject::LispObject(const MetaInfo& metaInfo)
    {
        m_header.m_pMetaInfo = &metaInfo;
    }

    template<typename T_Data>
    EZ_FORCE_INLINE
    T_Data& lcpp::LispObject::getData()
    {
        return reinterpret_cast<T_Data&>(m_typeSpecificData);
    }
}
