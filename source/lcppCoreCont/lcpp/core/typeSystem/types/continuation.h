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
        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParent, Function_t pFunction);

        LCPP_API_CORE_CONT void trampoline(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispRuntimeState> getRuntimeState(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT Ptr<LispObject> getParent(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT void setFunction(Ptr<LispObject> pCont, Function_t pFunction);

        LCPP_API_CORE_CONT Ptr<Stack> getStack(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT UserData_t& getUserData(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT void setUserData(Ptr<LispObject> pCont, UserData_t userData);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pToCall, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1, Ptr<LispObject> pArg2);

            Ptr<LispObject> tailCallHelper(Ptr<LispObject> pCont);
            Ptr<LispObject> tailCallHelper(Ptr<LispObject> pCont, Function_t pFunction);

            Ptr<LispObject> returnHelper(Ptr<LispObject> pCont);
            Ptr<LispObject> returnHelper(Ptr<LispObject> pCont, Ptr<LispObject> pReturnValue);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/continuationMacros.h"
#include "lcpp/core/typeSystem/types/impl/continuation.inl"
