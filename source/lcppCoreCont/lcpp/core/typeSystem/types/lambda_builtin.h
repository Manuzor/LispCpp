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

            LCPP_API_CORE_CONT StackPtr<LispObject> create(StackPtr<LispObject> pParentEnv, Function_t pFunction, const Signature& signature);

            LCPP_API_CORE_CONT StackPtr<LispObject> call(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT void checkArguments(StackPtr<LispObject> pLambda, StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(StackPtr<LispObject> pSyntax);

            LCPP_API_CORE_CONT StackPtr<LispObject> getName(StackPtr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(StackPtr<LispObject> pLambda, StackPtr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> getEnvironment(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT Function_t getFunction(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

            namespace detail
            {
                StackPtr<LispObject> call_finalize(StackPtr<LispObject> pCont);
            }
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambda_builtin.inl"
