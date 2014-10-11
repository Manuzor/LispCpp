#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace cont
    {
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
