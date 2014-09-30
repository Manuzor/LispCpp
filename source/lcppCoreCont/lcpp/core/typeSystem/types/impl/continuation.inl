#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace cont
    {
        namespace detail
        {
            EZ_FORCE_INLINE
            StackPtr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall)
            {
                return create(pCont, pToCall);
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                return pContNew;
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0, StackPtr<LispObject> pArg1)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                return pContNew;
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0, StackPtr<LispObject> pArg1, StackPtr<LispObject> pArg2)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                pStack->push(pArg2);
                return pContNew;
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> tailCallHelper(StackPtr<LispObject> pCont)
            {
                return pCont;
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> tailCallHelper(StackPtr<LispObject> pCont, Function_t pFunction)
            {
                cont::setFunction(pCont, pFunction);
                return pCont;
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> returnHelper(StackPtr<LispObject> pCont)
            {
                return getParent(pCont);
            }

            EZ_FORCE_INLINE
            StackPtr<LispObject> returnHelper(StackPtr<LispObject> pCont, StackPtr<LispObject> pReturnValue)
            {
                auto pContParent = getParent(pCont);
                getStack(pContParent)->push(pReturnValue);
                return pContParent;
            }
        }
    }
}
