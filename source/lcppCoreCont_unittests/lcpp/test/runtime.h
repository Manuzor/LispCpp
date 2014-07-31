#pragma once

namespace lcpp
{
    class LispRuntimeState;

    namespace test
    {
        Ptr<LispRuntimeState> getRuntimeState();
    }
}

#undef LCPP_test_pRuntimeState
#define LCPP_test_pRuntimeState ::lcpp::test::getRuntimeState()
