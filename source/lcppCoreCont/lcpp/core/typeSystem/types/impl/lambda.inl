
namespace lcpp
{
    namespace lambda
    {
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
