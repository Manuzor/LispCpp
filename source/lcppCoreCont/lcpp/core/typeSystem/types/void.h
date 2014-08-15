#pragma once

namespace lcpp
{
    class LispObject;

    namespace void_
    {

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

    };

    bool isVoid(Ptr<LispObject> pObject);
}

#define LCPP_pVoid ::lcpp::void_::create()

#include "lcpp/core/typeSystem/types/impl/void.inl"
