#pragma once
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/continuationCommon.h"

namespace lcpp
{
    class MetaInfo;
    class LispObject;
    class LispRuntimeState;

    namespace cont
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispRuntimeState> pRuntimeState);
        LCPP_API_CORE_CONT Ptr<LispObject> create(StackPtr<LispObject> pParent, Function_t pFunction);

        LCPP_API_CORE_CONT void trampoline(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispRuntimeState> getRuntimeState(StackPtr<LispObject> pCont);
        LCPP_API_CORE_CONT Ptr<LispObject> getParent(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT Function_t getFunction(StackPtr<LispObject> pCont);
        LCPP_API_CORE_CONT void setFunction(StackPtr<LispObject> pCont, Function_t pFunction);

        LCPP_API_CORE_CONT Ptr<Stack> getStack(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT UserData_t& getUserData(StackPtr<LispObject> pCont);
        LCPP_API_CORE_CONT void setUserData(StackPtr<LispObject> pCont, UserData_t userData);

        LCPP_API_CORE_CONT ezUInt32 getDepth(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0, StackPtr<LispObject> pArg1);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(StackPtr<LispObject> pCont, Function_t pToCall, StackPtr<LispObject> pArg0, StackPtr<LispObject> pArg1, StackPtr<LispObject> pArg2);

            Ptr<LispObject> tailCallHelper(StackPtr<LispObject> pCont);
            Ptr<LispObject> tailCallHelper(StackPtr<LispObject> pCont, Function_t pFunction);

            Ptr<LispObject> returnHelper(StackPtr<LispObject> pCont);
            Ptr<LispObject> returnHelper(StackPtr<LispObject> pCont, StackPtr<LispObject> pReturnValue);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/continuationMacros.h"
#include "lcpp/core/typeSystem/types/impl/continuation.inl"
