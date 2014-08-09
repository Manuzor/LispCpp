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

    Ptr<LispObject> readStream(Ptr<LispObject> pStream);

    Ptr<LispObject> readString(const ezString& content);

    Ptr<LispObject> evalStream(Ptr<LispObject> pStream);

    Ptr<LispObject> evalString(const ezString& content);

}} // namespace lcpp::test

#include "lcpp/test/impl/testGroupMacros.inl"
#include "lcpp/test/impl/testMacros.inl"

#include "lcpp/test/runtime.h"
