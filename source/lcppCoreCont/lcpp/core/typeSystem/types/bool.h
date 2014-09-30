#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace true_
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject);
    }

    namespace false_
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);
    }

    bool isTrue(StackPtr<LispObject> pObject);

    bool isFalse(StackPtr<LispObject> pObject);
}

#define LCPP_pTrue ::lcpp::true_::create()
#define LCPP_pFalse ::lcpp::false_::create()

#include "lcpp/core/typeSystem/types/impl/bool.inl"
