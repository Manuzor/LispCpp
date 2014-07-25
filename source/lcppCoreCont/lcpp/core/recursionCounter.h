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

#include "lcpp/core/impl/recursionCounter.inl"
