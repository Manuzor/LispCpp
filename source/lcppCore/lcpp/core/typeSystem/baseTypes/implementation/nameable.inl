inline
lcpp::NameableBase::NameableBase(Ptr<LispSymbol> pName) :
    m_pName(pName)
{
}

inline
bool
lcpp::NameableBase::hasName() const
{
    return !m_pName.isNull();
}

inline
void
lcpp::NameableBase::name(Ptr<LispSymbol> pNewName)
{
    m_pName = pNewName;
}

inline
lcpp::Ptr<const lcpp::LispSymbol>
lcpp::NameableBase::name() const
{
    return m_pName;
}

inline
lcpp::Ptr<lcpp::LispSymbol>
lcpp::NameableBase::name()
{
    return m_pName;
}

