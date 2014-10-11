
namespace lcpp
{
    namespace lambda
    {
        namespace userDefined
        {
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
            Ptr<LispObject> Data::getEnv()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pEnv);
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getArgList()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pArgList);
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getBody()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pBody);
            }
        }
    }
}
