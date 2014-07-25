#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class LCPP_API_CORE_CONT LispBool
    {
    public:

        static Ptr<LispObject> create(bool value);

    };
    
}
