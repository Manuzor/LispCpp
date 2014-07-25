#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class LispObject;

    namespace nil
    {

        LCPP_API_CORE_CONT Ptr<LispObject> create();

    };

    bool isNil(Ptr<LispObject> pObject);
}
#define LCPP_NIL ::lcpp::nil::create()

#include "lcpp/core/typeSystem/types/impl/nil.inl"
