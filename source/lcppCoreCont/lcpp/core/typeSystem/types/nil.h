#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class LispObject;

    class LispNil
    {
    public:

        static LCPP_API_CORE_CONT Ptr<LispObject> create();

    };

#define LCPP_NIL ::lcpp::LispNil::create()
}
