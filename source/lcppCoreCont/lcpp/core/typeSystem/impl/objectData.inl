
namespace lcpp
{
    inline
    LispObject::LispObject(const MetaInfo& metaInfo)
    {
        m_header.m_pMetaInfo = &metaInfo;
    }
}
