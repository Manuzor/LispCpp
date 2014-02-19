#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(StringUtil)
    {
        TEST_METHOD(ToString)
        {
            {
                auto str = lcpp::toString(1);
                Assert::IsTrue(str.IsEqual("1"));
            }
            {
                auto str = lcpp::toString(1U);
                Assert::IsTrue(str.IsEqual("1"));
            }
            {
                auto str = lcpp::toString(1.0f);
                Assert::IsTrue(str.IsEqual("1"));

                str = lcpp::toString(1.1f);
                Assert::IsTrue(str.IsEqual("1.1"));
            }
            {
                auto str = lcpp::toString(1.0);
                Assert::IsTrue(str.IsEqual("1"));

                str = lcpp::toString(1.1);
                Assert::IsTrue(str.IsEqual("1.1"));
            }
        }
    };

}}