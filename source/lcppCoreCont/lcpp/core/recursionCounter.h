#pragma once

namespace lcpp
{
    class LispRuntimeState;

    class RecursionCounter
    {
    public:
        Ptr<LispRuntimeState> pRunTime;

        RecursionCounter(Ptr<LispRuntimeState> pRunTime);
        ~RecursionCounter();
    };
}

#include "lcpp/core/impl/recursionCounter.inl"
