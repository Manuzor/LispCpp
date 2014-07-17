#pragma once

namespace
{
    inline
    lcpp::Ptr<lcpp::LispRuntime>
    resetRuntime()
    {
        auto pRuntime = lcpp::LispRuntime::instance();
        pRuntime->shutdown();
        pRuntime->initialize();
        return pRuntime;
    }
}
