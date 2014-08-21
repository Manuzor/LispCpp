#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace true_
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        Ptr<LispObject> toString(Ptr<LispObject> pObject);
    }

    namespace false_
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
    }

    bool isTrue(Ptr<LispObject> pObject);

    bool isFalse(Ptr<LispObject> pObject);
}

#define LCPP_pTrue ::lcpp::true_::create()
#define LCPP_pFalse ::lcpp::false_::create()

#include "lcpp/core/typeSystem/types/impl/bool.inl"
