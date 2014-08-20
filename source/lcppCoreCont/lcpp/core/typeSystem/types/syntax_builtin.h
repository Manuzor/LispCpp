#pragma once

#include "lcpp/core/typeSystem/types/syntaxCommon_builtin.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Signature;

    namespace syntax
    {
        namespace builtin
        {

            LCPP_API_CORE_CONT const MetaInfo& metaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Function_t pFunction, const Signature& signature);

            Ptr<LispObject> create(Function_t pFunction);

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT void checkArguments(Ptr<LispObject> pSyntax, Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pSyntax);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pSyntax, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntax_builtin.inl"
