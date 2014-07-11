#pragma once

namespace lcpp
{
    class SchemeRuntime;

    struct RecursionCounter
    {
        Ptr<SchemeRuntime> pRunTime;

        RecursionCounter(Ptr<SchemeRuntime> pRunTime);
        ~RecursionCounter();
    };
}

#include "lcpp/core/implementation/recursionCounter_inl.h"
