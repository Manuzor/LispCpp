#pragma once

namespace lcpp
{
    class LispRuntime;

    namespace test
    {
        Ptr<LispRuntime> getRuntime();
    }
}

#undef LCPP_test_pRuntime
#define LCPP_test_pRuntime ::lcpp::test::getRuntime()
