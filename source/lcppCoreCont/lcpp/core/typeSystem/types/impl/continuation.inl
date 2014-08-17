#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace cont
    {
        EZ_FORCE_INLINE
        Ptr<LispRuntimeState> Data::getRuntimeState()
        {
            return reinterpret_cast<Ptr<LispRuntimeState>&>(m_pRuntimeState);
        }

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

        EZ_FORCE_INLINE
        UserData_t& Data::getUserData()
        {
            return m_userData;
        }

        namespace detail
        {
            EZ_FORCE_INLINE
            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall)
            {
                return create(pCont, pToCall);
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                return pContNew;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                return pContNew;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1, Ptr<LispObject> pArg2)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                pStack->push(pArg2);
                return pContNew;
            }

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
