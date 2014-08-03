
namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getName()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pName);
            }

            EZ_FORCE_INLINE
            void Data::setName(Ptr<LispObject> pNewName)
            {
                reinterpret_cast<Ptr<LispObject>&>(m_pName) = pNewName;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getEnv()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pEnv);
            }

            EZ_FORCE_INLINE
            Function_t Data::getFunction()
            {
                return m_pFunction;
            }
        }
    }
}
