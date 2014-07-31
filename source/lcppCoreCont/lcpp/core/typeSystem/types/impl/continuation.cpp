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

        Ptr<LispObject> createTopLevel(Function_t pFunction)
        {
            return create(LCPP_pNil, pFunction);
        }

        Ptr<LispObject> create(Ptr<LispObject> m_pParent, Function_t pFunction)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            auto& data = pInstance->m_cont;

            new (data.m_pParent) Ptr<LispObject>(m_pParent);
            data.m_pFunction = pFunction;
            new (data.m_stack) Stack();

            return pInstance;
        }

        void trampoline(Ptr<LispObject> pCont)
        {
            if(isNil(pCont))
            {
                return;
            }

            typeCheck(pCont, Type::Continuation);

            while(true)
            {
                auto pFunction = getFunction(pCont);

                pCont = (*pFunction)(pCont);

                if(isNil(pCont)) { break; }

                typeCheck(pCont, Type::Continuation);
            }
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
    }
}
