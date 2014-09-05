
namespace lcpp
{
    EZ_FORCE_INLINE
    RefIndex RefIndex::invalidValue()
    {
        RefIndex value;
        value.m_bIsStatic = false;
        value.m_uiHash = 0xFF;
        value.m_uiIndex = 0xFFFFFF;
        return value;
    }
    
    EZ_FORCE_INLINE
    bool RefIndex::isValid() const
    {
        return *this != invalidValue();
    }
    
    EZ_FORCE_INLINE
    bool RefIndex::isStatic() const
    {
        return bool(m_bIsStatic);
    }

    EZ_FORCE_INLINE
    bool operator == (const RefIndex& lhs, const RefIndex& rhs)
    {
        return lhs.m_uiId == rhs.m_uiId;
    }

    EZ_FORCE_INLINE
    bool operator != (const RefIndex& lhs, const RefIndex& rhs)
    {
        return lhs.m_uiId != rhs.m_uiId;
    }
}
