#pragma once

#include "lcpp/core/functionUtils/signature.h"
#include "lcpp/core/typeSystem/types/lambdaCommon_builtin.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace builtin
        {
            LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(StackPtr<LispObject> pParentEnv, Function_t pFunction, const Signature& signature);

            LCPP_API_CORE_CONT Ptr<LispObject> call(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT void checkArguments(Ptr<LispObject> pLambda, Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambda_builtin.inl"
