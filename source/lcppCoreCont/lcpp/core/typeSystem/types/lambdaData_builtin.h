#pragma once

#include "lcpp/core/functionUtils/signature.h"
#include "lcpp/core/typeSystem/types/lambdaCommon_builtin.h"

namespace lcpp
{
    class LispObject;

    namespace lambda
    {
        namespace builtin
        {
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
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambdaData_builtin.inl"
