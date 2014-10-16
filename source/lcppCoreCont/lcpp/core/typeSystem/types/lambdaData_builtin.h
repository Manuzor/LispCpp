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
                Signature m_signature;
                Ptr<LispObject> m_pName;
                Ptr<LispObject> m_pEnv;
                Function_t m_pFunction;
            };
        }
    }
}
