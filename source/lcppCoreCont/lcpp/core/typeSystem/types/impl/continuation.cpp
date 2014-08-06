#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace cont
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Continuation, "continuation");
            return meta;
        }

        static Ptr<LispObject> breakTrampoline(Ptr<LispObject>)
        {
            return LCPP_pNil;
        }

        Ptr<LispObject> createTopLevel(Ptr<LispRuntimeState> pRuntimeState)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());
            auto& data = pInstance->m_cont;

            new (data.m_pRuntimeState) Ptr<LispRuntimeState>(pRuntimeState);
            new (data.m_pParent) Ptr<LispObject>(LCPP_pNil);
            data.m_pFunction = &breakTrampoline;
            new (data.m_stack) Stack();

            return pInstance;
        }

        Ptr<LispObject> create(Ptr<LispObject> pParent, Function_t pFunction)
        {
            typeCheck(pParent, Type::Continuation);

            auto pInstance = LispObject::create<Data>(metaInfo());
            auto& data = pInstance->m_cont;

            new (data.m_pRuntimeState) Ptr<LispRuntimeState>(getRuntimeState(pParent));
            new (data.m_pParent) Ptr<LispObject>(pParent);
            data.m_pFunction = pFunction;
            new (data.m_stack) Stack();
            data.m_userData = UserData_t();

            return pInstance;
        }

        void trampoline(Ptr<LispObject> pCont)
        {
            if(isNil(pCont)) { return; }

            typeCheck(pCont, Type::Continuation);

            while(true)
            {
                auto pFunction = getFunction(pCont);

                EZ_ASSERT(pFunction, "Invalid function pointer.");

                pCont = (*pFunction)(pCont);

                if(isNil(pCont)) { break; }

                typeCheck(pCont, Type::Continuation);
            }
        }

        Ptr<LispRuntimeState> getRuntimeState(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->m_cont.getRuntimeState();
        }

        Ptr<LispObject> getParent(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->m_cont.getParent();
        }

        Function_t getFunction(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->m_cont.getFunction();
        }

        void setFunction(Ptr<LispObject> pCont, Function_t pFunction)
        {
            typeCheck(pCont, Type::Continuation);

            pCont->m_cont.setFunction(pFunction);
        }

        Ptr<Stack> getStack(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return &pCont->m_cont.getStack();
        }

        UserData_t& getUserData(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->m_cont.getUserData();
        }

        void setUserData(Ptr<LispObject> pCont, UserData_t userData)
        {
            typeCheck(pCont, Type::Continuation);

             pCont->m_cont.getUserData() = userData;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Continuation);

            static auto pString = str::create("<continuation>");

            return pString;
        }

        namespace detail
        {
            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall)
            {
                return create(pCont, pToCall);
            }

            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                return pContNew;
            }

            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                return pContNew;
            }

            Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1, Ptr<LispObject> pArg2)
            {
                auto pContNew = create(pCont, pToCall);
                auto pStack = getStack(pContNew);
                pStack->push(pArg0);
                pStack->push(pArg1);
                pStack->push(pArg2);
                return pContNew;
            }
        }

    }
}
