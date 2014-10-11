
namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            EZ_FORCE_INLINE
            Ptr<Signature> Data::getSignature()
            {
                return &m_signature;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getName()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pName);
            }

            EZ_FORCE_INLINE
            void Data::setName(Ptr<LispObject> pNewName)
            {
                reinterpret_cast<Ptr<LispObject>&>(m_pName) = pNewName.get();
            }

            EZ_FORCE_INLINE
            Function_t Data::getFunction()
            {
                return m_pFunction;
            }
        }
    }
}
