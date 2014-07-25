#pragma once

namespace lcpp
{
    class LispObject;

    namespace true_
    {
        LCPP_API_CORE_CONT Ptr<LispObject> create();
    }

    namespace false_
    {
        LCPP_API_CORE_CONT Ptr<LispObject> create();
    }

    bool isTrue(Ptr<LispObject> pObject);

    bool isFalse(Ptr<LispObject> pObject);

}

#define LCPP_TRUE ::lcpp::true_::create()
#define LCPP_FALSE ::lcpp::false_::create()

#include "lcpp/core/typeSystem/types/impl/bool.inl"
