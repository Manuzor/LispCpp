#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            const MetaInfo& metaInfo()
            {
                static auto meta = MetaInfo(Type::Lambda,
                                            AttributeFlags::Callable | AttributeFlags::Builtin,
                                            "builtin-lambda");

                return meta;
            }

            Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Function_t pFunction)
            {
                typeCheck(pParentEnv, Type::Environment);

                auto pInstance = LispObject::create<Data>(metaInfo());
                auto& data = pInstance->m_lambda_builtin;

                auto pLocalEnv = env::createAnonymous(pParentEnv);

                new (data.m_pEnv) Ptr<LispObject>(pLocalEnv);
                data.m_pFunction = pFunction;

                return pInstance;
            }

            Ptr<LispObject> call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pLambda = pStack->get(-1);
                pStack->pop();

                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                auto pFunction = getFunction(pLambda);

                LCPP_cont_tailCall(pCont, pFunction);
            }

            Function_t getFunction(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_builtin.getFunction();
            }
        }
    }
}
