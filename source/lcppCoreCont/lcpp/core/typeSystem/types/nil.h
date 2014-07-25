#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class LispObject;

    namespace nil
    {

        LCPP_API_CORE_CONT Ptr<LispObject> create();

    };
}
#define LCPP_NIL ::lcpp::LispNil::create()
