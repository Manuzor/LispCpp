#pragma once

namespace lcpp { namespace test {

    class UnitTest :
        public cut::IUnitTest
    {
    public:

        UnitTest(cut::UnitTestGroup& group);

        void inititialize();
        void shutdown();

        virtual void run() override
        {
            inititialize();
            LCPP_SCOPE_EXIT{ shutdown(); };
            doRun();
        }

        virtual void doRun() = 0;
    };

}} // namespace lcpp::test

#include "lcpp/test/impl/testGroupMacros.inl"
#include "lcpp/test/impl/testMacros.inl"
