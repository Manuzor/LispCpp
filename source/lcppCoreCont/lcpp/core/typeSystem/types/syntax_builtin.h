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

            LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

            LCPP_API_CORE_CONT StackPtr<LispObject> create(Function_t pFunction, const Signature& signature);

            StackPtr<LispObject> create(Function_t pFunction);

            LCPP_API_CORE_CONT StackPtr<LispObject> call(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT void checkArguments(StackPtr<LispObject> pSyntax, StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<Signature> getSignature(StackPtr<LispObject> pSyntax);

            LCPP_API_CORE_CONT StackPtr<LispObject> getName(Ptr<LispObject> pSyntax);
            LCPP_API_CORE_CONT void setName(StackPtr<LispObject> pSyntax, StackPtr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pSyntax);

            LCPP_API_CORE_CONT Function_t getFunction(StackPtr<LispObject> pSyntax);

            LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntax_builtin.inl"
