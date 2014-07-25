#pragma once

#include "lcpp/core/typeSystem/type.h"

namespace lcpp
{
    class LispSymbol
    {
    public:

        static LCPP_API_CORE_CONT Ptr<LispObject> create(const String& value);

    };
}
