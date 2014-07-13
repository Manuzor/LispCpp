#pragma once

namespace
{
    lcpp::Ptr<lcpp::LispRuntime> createTestRuntime()
    {
        auto pRuntime = lcpp::LispRuntime::instance();
        pRuntime->shutdown();
        pRuntime->initialize();
        return pRuntime;
    }
}
