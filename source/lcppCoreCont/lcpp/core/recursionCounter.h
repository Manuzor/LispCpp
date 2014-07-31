#pragma once

namespace lcpp
{
    class LispRuntime;

    class RecursionCounter
    {
    public:
        Ptr<LispRuntime> pRunTime;

        RecursionCounter(Ptr<LispRuntime> pRunTime);
        ~RecursionCounter();
    };
}

#include "lcpp/core/impl/recursionCounter.inl"
