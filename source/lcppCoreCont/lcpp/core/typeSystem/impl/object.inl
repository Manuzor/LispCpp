
namespace lcpp
{
    inline
    bool LispObject::isType(const Type& type) const
    {
        return getType() == type;
    }

    inline
    const Type& LispObject::getType() const
    {
        EZ_ASSERT(m_pTypeInfo, "No valid type info.");
        return m_pTypeInfo->getType();
    }
}
