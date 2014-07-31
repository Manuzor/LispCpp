
namespace lcpp
{
    namespace cont
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> Data::getParent()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pParent);
        }

        EZ_FORCE_INLINE
        Function_t Data::getFunction()
        {
            return m_pFunction;
        }

        EZ_FORCE_INLINE
        void Data::setFunction(Function_t pFunction)
        {
            m_pFunction = pFunction;
        }

        EZ_FORCE_INLINE
        Stack& Data::getStack()
        {
            return reinterpret_cast<Stack&>(m_stack);
        }

        namespace detail
        {
            EZ_FORCE_INLINE
            Ptr<LispObject> tailCallHelper(Ptr<LispObject> pCont)
            {
                return pCont;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> tailCallHelper(Ptr<LispObject> pCont, Function_t pFunction)
            {
                cont::setFunction(pCont, pFunction);
                return pCont;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> returnHelper(Ptr<LispObject> pCont)
            {
                return getParent(pCont);
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> returnHelper(Ptr<LispObject> pCont, Ptr<LispObject> pReturnValue)
            {
                auto pContParent = getParent(pCont);
                getStack(pContParent)->push(pReturnValue);
                return pContParent;
            }
        }
    }
}
