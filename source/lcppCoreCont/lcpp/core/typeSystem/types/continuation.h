#pragma once
#include "lcpp/core/containers/stack.h"

namespace lcpp
{
    class MetaInfo;
    class LispObject;

    namespace cont
    {
        typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel();
        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParent, Function_t pFunction);

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            Ptr<LispObject> getParent();

            Function_t getFunction();
            void setFunction(Function_t pFunction);

            Stack& getStack();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pParent);
            Function_t m_pFunction;
            LCPP_DeclareRawDataMember(Stack, m_stack);
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT void trampoline(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispObject> getParent(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pCont);
        LCPP_API_CORE_CONT void setFunction(Ptr<LispObject> pCont, Function_t pFunction);

        LCPP_API_CORE_CONT Ptr<Stack> getStack(Ptr<LispObject> pCont);

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pFunction);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pFunction, Ptr<LispObject> pArg0);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pFunction, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1);
            LCPP_API_CORE_CONT Ptr<LispObject> callHelper(Ptr<LispObject> pCont, Function_t pFunction, Ptr<LispObject> pArg0, Ptr<LispObject> pArg1, Ptr<LispObject> pArg2);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/continuationMacros.h"
#include "lcpp/core/typeSystem/types/impl/continuation.inl"
