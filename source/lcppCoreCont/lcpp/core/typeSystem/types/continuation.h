#pragma once
#include "lcpp/core/containers/stack.h"

namespace lcpp
{
    class MetaInfo;
    class LispObject;
    class LispRuntimeState;

    namespace cont
    {
        typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispRuntimeState> pRuntimeState);
        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParent, Function_t pFunction);

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            Ptr<LispRuntimeState> getRuntimeState();
            Ptr<LispObject> getParent();

            Function_t getFunction();
            void setFunction(Function_t pFunction);

            Stack& getStack();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispRuntimeState>, m_pRuntimeState);
            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pParent);
            Function_t m_pFunction;
            LCPP_DeclareRawDataMember(Stack, m_stack);
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT void trampoline(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispRuntimeState> getRuntimeState(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT Ptr<LispObject> getParent(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT void setFunction(Ptr<LispObject> pCont, Function_t pFunction);

        LCPP_API_CORE_CONT Ptr<Stack> getStack(Ptr<LispObject> pCont);

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
