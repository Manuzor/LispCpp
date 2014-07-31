#pragma once

namespace lcpp
{
    class LispRuntimeState;

    class RecursionCounter
    {
    public:
        Ptr<LispRuntimeState> m_pRunTimeState;

        RecursionCounter(Ptr<LispRuntimeState> m_pRunTimeState);
        ~RecursionCounter();
    };
}

#include "lcpp/core/impl/recursionCounter.inl"
