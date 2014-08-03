#pragma once

#include "lcpp/core/functionUtils.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {

            typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

            LCPP_API_CORE_CONT const MetaInfo& metaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Function_t pFunction, const Signature& signature);

            Ptr<LispObject> create(Function_t pFunction);

            //////////////////////////////////////////////////////////////////////////

            class Data
            {
            public:

                Ptr<Signature> getSignature();
                Function_t getFunction();

            public:

                Signature m_signature;
                Function_t m_pFunction;

            };

            //////////////////////////////////////////////////////////////////////////

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT void checkArguments(Ptr<LispObject> pSyntax, Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT String toString(Ptr<LispObject> pObject);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntax_builtin.inl"
