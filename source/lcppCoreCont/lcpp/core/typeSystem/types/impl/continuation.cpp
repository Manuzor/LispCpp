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
#include "lcpp/core/exceptions/runtimeException.h"

namespace lcpp
{
    namespace cont
    {
        static void scan(CollectableBase* pCollectable, GarbageCollectionContext* pGC)
        {
            auto pCont = static_cast<LispObject*>(pCollectable);
            typeCheck(pCont, Type::Continuation);

            auto& pParent = pCont->getData<Data>().m_pParent.get();
            pParent = pGC->addSurvivor(pParent);

            auto& stack = pCont->getData<Data>().m_stack;
            for(size_t i = 0; i < stack.size(); i++)
            {
                auto& pToPatch = stack.get(i).get();
                pToPatch = pGC->addSurvivor(pToPatch);
            }

        }

        static void destroy(lcpp::CollectableBase* pCollectable)
        {
            auto pCont = static_cast<LispObject*>(pCollectable);
            typeCheck(pCont, Type::Continuation);

            pCont->getData<Data>().m_stack.~Stack();
        }

        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            typeCheck(pLhs, Type::Environment);
            return pLhs == pRhs;
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Continuation);
                meta.setPrettyName("continuation");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction,
                                              static_cast<ScanFunction_t>(&scan)));
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction,
                                              static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        static Ptr<LispObject> breakTrampoline(StackPtr<LispObject>)
        {
            return LCPP_pNil;
        }

        Ptr<LispObject> createTopLevel(Ptr<LispRuntimeState> pRuntimeState)
        {
            auto pInstance = object::create<Data>(getMetaInfo());

            LCPP_GC_PreventCollectionInScope;
            auto& data = pInstance->getData<Data>();

            data.m_pRuntimeState = pRuntimeState;
            data.m_pParent = LCPP_pNil;
            data.m_pFunction = &breakTrampoline;
            data.m_userData = 0;

            return pInstance;
        }

        Ptr<LispObject> create(StackPtr<LispObject> pParent, Function_t pFunction)
        {
            LCPP_LogBlock("cont::create");

            typeCheck(pParent, Type::Continuation);

            auto pInstance = object::create<Data>(getMetaInfo());
            auto& data = pInstance->getData<Data>();

            data.m_pRuntimeState = getRuntimeState(pParent);
            data.m_pParent = pParent.get();
            data.m_pFunction = pFunction;
            data.m_userData = 0;

            return pInstance;
        }

        void trampoline(StackPtr<LispObject> pCont)
        {
            if(isNil(pCont)) { return; }

            typeCheck(pCont, Type::Continuation);
            auto pState = getRuntimeState(pCont);
            pState->setBreakExecution(false);

            while(true)
            {
                auto pFunction = getFunction(pCont);

                EZ_ASSERT(pFunction, "Invalid function pointer.");

                pCont = (*pFunction)(pCont);

                if(isNil(pCont)) { break; }
                if(pState->getBreakExecution())
                {
                    pState->setBreakExecution(false);
                    LCPP_THROW(exceptions::Runtime("Execution was aborted."));
                }

                typeCheck(pCont, Type::Continuation);
            }
        }

        Ptr<LispRuntimeState> getRuntimeState(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().m_pRuntimeState;
        }

        Ptr<LispObject> getParent(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().m_pParent;
        }

        Function_t getFunction(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().m_pFunction;
        }

        void setFunction(StackPtr<LispObject> pCont, Function_t pFunction)
        {
            typeCheck(pCont, Type::Continuation);

            pCont->getData<Data>().m_pFunction = pFunction;
        }

        Ptr<Stack> getStack(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return &pCont->getData<Data>().m_stack;
        }

        UserData_t& getUserData(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            return pCont->getData<Data>().m_userData;
        }

        void setUserData(StackPtr<LispObject> pCont, UserData_t userData)
        {
            typeCheck(pCont, Type::Continuation);

            pCont->getData<Data>().m_userData = userData;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Continuation);
            return str::create("<continuation>");
        }
    }
}
