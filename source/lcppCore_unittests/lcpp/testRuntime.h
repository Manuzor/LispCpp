#pragma once

namespace
{
    lcpp::Ptr<lcpp::SchemeRuntime> createTestRuntime()
    {
        lcpp::Ptr<lcpp::SchemeRuntime> pRuntime =
            LCPP_NEW(lcpp::defaultAllocator(),
                     lcpp::SchemeRuntime);
        pRuntime->initialize();
        return pRuntime;
    }
}
