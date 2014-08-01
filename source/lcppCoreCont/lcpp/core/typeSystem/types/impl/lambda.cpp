#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/typeCheck.h"

namespace lcpp
{
    namespace lambda
    {

        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Lambda, "lambda");

            return meta;
        }

        Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Function_t pFunction)
        {
            typeCheck(pParentEnv, Type::Environment);

            auto pInstance = LispObject::create<Data>(metaInfo());
            auto& data = pInstance->m_lambda;

            auto pLocalEnv = env::createAnonymous(pParentEnv);

            new (data.m_pEnv) Ptr<LispObject>(pLocalEnv);
            data.m_pFunction = pFunction;

            return pInstance;
        }

        Ptr<LispObject> call(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            auto pLambda = cont::getStack(pCont)->get(-1);

            auto pFunction = getFunction(pLambda);

            LCPP_NOT_IMPLEMENTED;
        }

        Function_t getFunction(Ptr<LispObject> pLambda)
        {
            typeCheck(pLambda, Type::Lambda);

            return pLambda->m_lambda.getFunction();
        }

        namespace detail
        {
            Ptr<LispObject> userDefinedFunction(Ptr<LispObject> pCont)
            {
                LCPP_NOT_IMPLEMENTED;
            }
        }
    }
}
