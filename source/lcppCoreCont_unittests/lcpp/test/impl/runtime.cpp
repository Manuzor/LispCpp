#include "stdafx.h"
#include "lcpp/test/runtime.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace test
    {
        Ptr<LispRuntime> getRuntime()
        {
            static auto pRuntime = Ptr<LispRuntime>();

            if (!pRuntime)
            {
                static auto runtime = LispRuntime();
                pRuntime = &runtime;
            }

            return pRuntime;
        }
    }
}
