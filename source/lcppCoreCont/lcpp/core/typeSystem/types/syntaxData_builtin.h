#pragma once

#include "lcpp/core/functionUtils/signature.h"
#include "lcpp/core/typeSystem/types/syntaxCommon_builtin.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {
            class Data
            {
            public:
                Signature m_signature;
                Ptr<LispObject> m_pName;
                Function_t m_pFunction;
            };
        }
    }
}
