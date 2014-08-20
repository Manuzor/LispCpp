#pragma once
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/continuationCommon.h"

namespace lcpp
{
    class LispObject;
    class LispRuntimeState;

    namespace cont
    {
        class Data
        {
        public:

            Ptr<LispRuntimeState> getRuntimeState();
            Ptr<LispObject> getParent();

            Function_t getFunction();
            void setFunction(Function_t pFunction);

            Stack& getStack();

            UserData_t& getUserData();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispRuntimeState>, m_pRuntimeState);
            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pParent);
            Function_t m_pFunction;
            LCPP_DeclareRawDataMember(Stack, m_stack);
            UserData_t m_userData;
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/continuationData.inl"
