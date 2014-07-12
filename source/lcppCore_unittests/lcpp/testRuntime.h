#pragma once

namespace
{
    lcpp::Ptr<lcpp::LispRuntime> createTestRuntime()
    {
        lcpp::Ptr<lcpp::LispRuntime> pRuntime =
            LCPP_NEW(lcpp::defaultAllocator(),
                     lcpp::LispRuntime);
        pRuntime->initialize();
        return pRuntime;
    }
}
