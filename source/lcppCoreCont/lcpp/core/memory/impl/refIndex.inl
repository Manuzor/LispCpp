
namespace lcpp
{
    EZ_FORCE_INLINE
    RefIndex RefIndex::invalidValue()
    {
        RefIndex value;
        value.m_uiIndex = std::numeric_limits<decltype(value.m_uiIndex)>::max();
        return value;
    }

    EZ_FORCE_INLINE
    bool operator == (const RefIndex& lhs, const RefIndex& rhs)
    {
        return lhs.m_uiIndex == rhs.m_uiIndex;
    }
}
