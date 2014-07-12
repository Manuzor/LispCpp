#pragma once

namespace lcpp
{
    class LispRuntime;

    struct RecursionCounter
    {
        Ptr<LispRuntime> pRunTime;

        RecursionCounter(Ptr<LispRuntime> pRunTime);
        ~RecursionCounter();
    };
}

#include "lcpp/core/implementation/recursionCounter_inl.h"
