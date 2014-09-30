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

                Ptr<Signature> getSignature();

                Ptr<LispObject> getName();
                void setName(StackPtr<LispObject> pNewName);

                Function_t getFunction();

            public:

                Signature m_signature;
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);
                Function_t m_pFunction;

            };
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntaxData_builtin.inl"
