#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

#pragma warning( push )
#pragma warning( disable : 4244 ) // conversion from 'x' to 'y', possible loss of data

namespace
{
    UnitTestGroup g_group("LispNumberTests");

    UnitTest g_test1(g_group, "Construction", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        union
        {
            ezUInt32 i;
            float f;
        } test;

        // LispInteger
        { // 1
            auto int1 = factory.createInteger(1);
            CUT_ASSERT.isTrue(int1->value() == 1);
        }
        { // 2
            auto int2 = factory.createInteger(2);
            CUT_ASSERT.isTrue(int2->value() == 2);
        }
        { // min
            auto intMin = factory.createInteger(-0xFFFF);
            CUT_ASSERT.isTrue(intMin->value() < 0);
            CUT_ASSERT.isTrue(intMin->value() == -0xFFFF);
        }
        { // max
            auto intMax = factory.createInteger(0xFFFF);
            CUT_ASSERT.isTrue(intMax->value() > 0);
            CUT_ASSERT.isTrue(intMax->value() == 0xFFFF);
        }

        // LispNumber
        { // 1.0f
            auto float1_0 = factory.createNumber(1.0f);
            CUT_ASSERT.isTrue(float1_0->value() == 1.0f);
        }
        { // 2.0f
            auto float2_0 = factory.createNumber(2.0f);
            CUT_ASSERT.isTrue(float2_0->value() == 2.0f);
        }
        { // min
            auto floatMin = factory.createNumber(0.0001f);
            CUT_ASSERT.isTrue(floatMin->value() == 0.0001f);
        }
        { // max
            auto floatMax = factory.createNumber(1337.0f);
            CUT_ASSERT.isTrue(floatMax->value() == 1337.0f);
        }
        { // first 255 numbers
            for(test.i = 0; test.i < 0xFF; test.i++)
            {
                auto sf = factory.createNumber(test.f);
                CUT_ASSERT.isTrue(sf->value() == test.f);
            }
        }
    });

    UnitTest g_test2(g_group, "ToString", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();
        // LispInteger
        {
            auto int1 = factory.createInteger(1);
            ezString str = int1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "LispInteger::toString is broken!");
        }
        {
            auto int1337 = factory.createInteger(1337);
            ezString str = int1337->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1337"), "LispInteger::toString is broken!");
        }
        {
            auto intMax = factory.createInteger(std::numeric_limits<LispInteger::Number_t>::max());
            ezString str = intMax->toString();
            CUT_ASSERT.isTrue(str.IsEqual("9223372036854775807"), "LispInteger::toString is broken!");
        }
        {
            auto intMin = factory.createInteger(std::numeric_limits<LispInteger::Number_t>::min());
            ezString str = intMin->toString();
            CUT_ASSERT.isTrue(str.IsEqual("-9223372036854775808"), "LispInteger::toString is broken!");
        }

        // LispNumber
        {
            auto number1 = factory.createNumber(1.0f);
            ezString str = number1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "LispNumber::toString is broken!");
        }
        {
            auto number1 = factory.createNumber(1.1f);
            ezString str = number1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1.1"), "LispNumber::toString is broken!");
        }
    });

    UnitTest g_test3(g_group, "Instancing", []{
        auto pRuntime = createTestRuntime();
        auto& factory = *pRuntime->factory();

        auto int1 = factory.createInteger(42);
        auto int2 = factory.createInteger(42);

        CUT_ASSERT.isTrue(int1 == int2, "Scheme integers with the same value should be the same object!");
    });
}

#pragma warning( pop )
