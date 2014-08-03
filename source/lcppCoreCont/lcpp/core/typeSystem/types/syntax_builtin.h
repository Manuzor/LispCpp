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

                Ptr<LispObject> getName();
                void setName(Ptr<LispObject> pNewName);

                Function_t getFunction();

            public:

                Signature m_signature;
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);
                Function_t m_pFunction;

            };

            //////////////////////////////////////////////////////////////////////////

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pSyntax);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pSyntax, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT void checkArguments(Ptr<LispObject> pSyntax, Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntax_builtin.inl"
