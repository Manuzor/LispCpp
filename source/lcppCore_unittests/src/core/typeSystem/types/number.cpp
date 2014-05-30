#include "stdafx.h"

using namespace cut;
using namespace lcpp;

#pragma warning( push )
#pragma warning( disable : 4244 ) // conversion from 'x' to 'y', possible loss of data

namespace
{
    UnitTestGroup g_group("SchemeNumberTests");

    UnitTest g_test1(g_group, "Construction", [](){
        TypeFactory factory;
        union
        {
            ezUInt32 i;
            float f;
        } test;

        // SchemeInteger
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

        // SchemeNumber
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
            for (test.i = 0; test.i < 0xFF; test.i++)
            {
                auto sf = factory.createNumber(test.f);
                CUT_ASSERT.isTrue(sf->value() == test.f);
            }
        }
    });

    UnitTest g_test2(g_group, "ToString", [](){
        TypeFactory factory;
        // SchemeInteger
        {
            auto int1 = factory.createInteger(1);
            ezString str = int1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeInteger::toString is broken!");
        }
        {
            auto int1337 = factory.createInteger(1337);
            ezString str = int1337->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1337"), "SchemeInteger::toString is broken!");
        }
        {
            auto intMax = factory.createInteger(std::numeric_limits<SchemeInteger::Number_t>::max());
            ezString str = intMax->toString();
            CUT_ASSERT.isTrue(str.IsEqual("9223372036854775807"), "SchemeInteger::toString is broken!");
        }
        {
            auto intMin = factory.createInteger(std::numeric_limits<SchemeInteger::Number_t>::min());
            ezString str = intMin->toString();
            CUT_ASSERT.isTrue(str.IsEqual("-9223372036854775808"), "SchemeInteger::toString is broken!");
        }

        // SchemeNumber
        {
            auto number1 = factory.createNumber(1.0f);
            ezString str = number1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeNumber::toString is broken!");
        }
        {
            auto number1 = factory.createNumber(1.1f);
            ezString str = number1->toString();
            CUT_ASSERT.isTrue(str.IsEqual("1.1"), "SchemeNumber::toString is broken!");
        }
    });

    UnitTest g_test3(g_group, "Instancing", [](){
        TypeFactory factory;

        auto int1 = factory.createInteger(42);
        auto int2 = factory.createInteger(42);

        CUT_ASSERT.isTrue(int1 == int2, "Scheme integers with the same value should be the same object!");
    });
}

#pragma warning( pop )
