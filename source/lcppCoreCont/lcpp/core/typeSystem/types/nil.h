#pragma once

namespace lcpp
{
    class LispObject;

    namespace nil
    {

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT String toString(Ptr<LispObject> pObject);

    };

    bool isNil(Ptr<LispObject> pObject);
}

#define LCPP_pNil ::lcpp::nil::create()

#include "lcpp/core/typeSystem/types/impl/nil.inl"
