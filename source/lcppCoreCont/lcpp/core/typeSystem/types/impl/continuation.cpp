#include "stdafx.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/continuationData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/types/nil.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace cont
    {
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Continuation);
                meta.setPrettyName("continuation");

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        static StackPtr<LispObject> breakTrampoline(StackPtr<LispObject>)
        {
            return LCPP_pNil;
        }

        StackPtr<LispObject> createTopLevel(Ptr<LispRuntimeState> pRuntimeState)
        {
            auto pInstance = object::create<Data>(getMetaInfo());
            auto& data = pInstance->getData<Data>();

            new (data.m_pRuntimeState) Ptr<LispRuntimeState>(pRuntimeState);
            new (data.m_pParent) Ptr<LispObject>(LCPP_pNil);
            data.m_pFunction = &breakTrampoline;
            new (data.m_stack) Stack();

            return pInstance;
        }

        StackPtr<LispObject> create(StackPtr<LispObject> pParent, Function_t pFunction)
        {
            LCPP_LogBlock("cont::create");

            typeCheck(pParent, Type::Continuation);

            auto pInstance = object::create<Data>(getMetaInfo());
            auto& data = pInstance->getData<Data>();

            new (data.m_pRuntimeState) Ptr<LispRuntimeState>(getRuntimeState(pParent));
            new (data.m_pParent) Ptr<LispObject>(pParent);
            data.m_pFunction = pFunction;
            new (data.m_stack) Stack();
            data.m_userData = UserData_t();

            return pInstance;
        }

        void trampoline(StackPtr<LispObject> pCont)
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

        Ptr<LispRuntimeState> getRuntimeState(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().getRuntimeState();
        }

        StackPtr<LispObject> getParent(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().getParent();
        }

        Function_t getFunction(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().getFunction();
        }

        void setFunction(StackPtr<LispObject> pCont, Function_t pFunction)
        {
            typeCheck(pCont, Type::Continuation);

            pCont->getData<Data>().setFunction(pFunction);
        }

        Ptr<Stack> getStack(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return &pCont->getData<Data>().getStack();
        }

        UserData_t& getUserData(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().getUserData();
        }

        void setUserData(StackPtr<LispObject> pCont, UserData_t userData)
        {
            typeCheck(pCont, Type::Continuation);

            pCont->getData<Data>().getUserData() = userData;
        }

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Continuation);

            static auto pString = str::create("<continuation>");

            return pString;
        }
    }
}
