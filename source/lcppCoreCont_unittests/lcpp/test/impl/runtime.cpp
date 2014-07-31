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
                static auto runtime = LispRuntimeState();
                pRuntime = &runtime;
            }

            return pRuntime;
        }
    }
}
