#include "stdafx.h"
#include "lcpp/test/runtime.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace test
    {
        Ptr<LispRuntimeState> getRuntimeState()
        {
            static auto pRuntime = Ptr<LispRuntimeState>();

            if (!pRuntime)
            {
                static LispRuntimeState runtime;
                pRuntime = &runtime;
            }

            return pRuntime;
        }
    }
}
