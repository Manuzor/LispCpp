#pragma once

#include "lcpp/core/functionUtils/signature.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace builtin
        {

            typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

            LCPP_API_CORE_CONT const MetaInfo& metaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Function_t pFunction, const Signature& signature);

            //////////////////////////////////////////////////////////////////////////

            class Data
            {
            public:

                Ptr<Signature> getSignature();

                Ptr<LispObject> getName();
                void setName(Ptr<LispObject> pNewName);

                Ptr<LispObject> getEnv();
                Function_t getFunction();

            public:

                Signature m_signature;
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pEnv);
                Function_t m_pFunction;

            };

            //////////////////////////////////////////////////////////////////////////

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT void checkArguments(Ptr<LispObject> pLambda, Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

            namespace detail
            {
                Ptr<LispObject> call_finalize(Ptr<LispObject> pCont);
            }
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambda_builtin.inl"
