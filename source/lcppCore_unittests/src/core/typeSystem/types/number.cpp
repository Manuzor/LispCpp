#include "stdafx.h"

using namespace cut;
using namespace lcpp;

#pragma warning( push )
#pragma warning( disable : 4244 ) // conversion from 'x' to 'y', possible loss of data

namespace
{
    UnitTestGroup g_group("SchemeNumberTests");

    UnitTest g_test1(g_group, "Construction", [](){
        union
        {
            ezUInt32 i;
            float f;
        } test;

        // SchemeInt32
        { // 1
            SchemeInt32 int1 = 1;
            CUT_ASSERT.isTrue(int1.value() == 1);
        }
        { // 2
            SchemeInt32 int2(2);
            CUT_ASSERT.isTrue(int2.value() == 2);
        }
        { // min
            SchemeInt32 intMin(-0xFFFF);
            CUT_ASSERT.isTrue(intMin.value() < 0);
            CUT_ASSERT.isTrue(intMin.value() == -0xFFFF);
        }
        { // max
            SchemeInt32 intMax(0xFFFF);
            CUT_ASSERT.isTrue(intMax.value() > 0);
            CUT_ASSERT.isTrue(intMax.value() == 0xFFFF);
        }

        // SchemeUInt32
        { // 1
            SchemeUInt32 uint1 = 1;
            CUT_ASSERT.isTrue(uint1.value() == 1U);
        }
        { // 2
            SchemeUInt32 uint2(2);
            CUT_ASSERT.isTrue(uint2.value() == 2U);
        }
        { // min
            SchemeUInt32 uintMin(0U);
            CUT_ASSERT.isTrue(uintMin.value() == 0U);
        }
        { // max
            SchemeUInt32 uintMax(0xFFFFFFFFU);
            CUT_ASSERT.isTrue(uintMax.value() == 0xFFFFFFFFU);
            CUT_ASSERT.isTrue(uintMax.value() == static_cast<ezUInt32>(-1));
        }

        // SchemeFloat
        { // 1.0f
            SchemeFloat float1_0 = 1.0f;
            CUT_ASSERT.isTrue(float1_0.value() == 1.0f);
        }
        { // 2.0f
            SchemeFloat float2_0(2.0f);
            CUT_ASSERT.isTrue(float2_0.value() == 2.0f);
        }
        { // min
            SchemeFloat floatMin(0.0001f);
            CUT_ASSERT.isTrue(floatMin.value() == 0.0001f);
        }
        { // max
            SchemeFloat floatMax(1337.0f);
            CUT_ASSERT.isTrue(floatMax.value() == 1337.0f);
        }
        { // first 255 numbers
            for (test.i = 0; test.i < 0xFF; test.i++)
            {
                SchemeFloat sf = test.f;
                CUT_ASSERT.isTrue(sf.value() == test.f);
            }
        }
    });

    UnitTest g_test2(g_group, "ToString", [](){
        // SchemeInt32
        {
            SchemeInt32 int1 = 1;
            ezString str = int1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeInt32::toString is broken!");
        }
        {
            SchemeInt32 int1337 = 1337;
            ezString str = int1337.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1337"), "SchemeInt32::toString is broken!");
        }
        {
            SchemeInt32 intMax = std::numeric_limits<SchemeInt32::Number_t>::max();
            ezString str = intMax.toString();
            CUT_ASSERT.isTrue(str.IsEqual("2147483647"), "SchemeInt32::toString is broken!");
        }
        {
            SchemeInt32 intMin = std::numeric_limits<SchemeInt32::Number_t>::min();
            ezString str = intMin.toString();
            CUT_ASSERT.isTrue(str.IsEqual("-2147483648"), "SchemeInt32::toString is broken!");
        }

        // SchemeUInt32
        {
            SchemeUInt32 uint1 = 1;
            ezString str = uint1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeUInt32::toString is broken!");
        }
        {
            SchemeUInt32 uint1337 = 1337;
            ezString str = uint1337.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1337"), "SchemeInt32::toString is broken!");
        }
        {
            SchemeUInt32 uintMax = std::numeric_limits<SchemeUInt32::Number_t>::max();
            ezString str = uintMax.toString();
            CUT_ASSERT.isTrue(str.IsEqual("4294967295"), "SchemeInt32::toString is broken!");
        }
        {
            SchemeUInt32 uintMin = std::numeric_limits<SchemeUInt32::Number_t>::min();
            ezString str = uintMin.toString();
            CUT_ASSERT.isTrue(str.IsEqual("0"), "SchemeInt32::toString is broken!");
        }

        // SchemeFloat
        {
            SchemeFloat float1 = 1.0f;
            ezString str = float1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeFloat::toString is broken!");
        }
        {
            SchemeFloat float1 = 1.1f;
            ezString str = float1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1.1"), "SchemeFloat::toString is broken!");
        }
    });

    UnitTest g_test3(g_group, "Conversion", [](){
        SchemeInt32 theInt = 42;
        CUT_ASSERT.isTrue(ezInt32(theInt) == ezInt32(42), "Explicit conversion does not work!");
        auto equals = [](ezInt32 lhs, ezInt32 rhs){ return lhs == rhs; };
        CUT_ASSERT.isTrue(equals(theInt, 42), "Implicit conversion does not work!");
    });

    UnitTest g_test4(g_group, "Operator_Assign", [](){
        SchemeInt32 first = 1;
        SchemeInt32 second = 2;
        SchemeFloat third = 3.1415f;

        first = 3;
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 3,
                          "Assignment operator of SchemeNumber_t broken!");

        first = 7.123;
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 7,
                          "Failed to assign a float to a SchemeInt32!");

        first = second;
        CUT_ASSERT.isTrue(first == second,
                          "Failed to assign a SchemeInt32 to a SchemeInt32!");

        first = third;
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == SchemeInt32::Number_t(third),
                                                 "Failed to assign a SchemeFloat to a SchemeInt32!");
    });

    UnitTest g_test5(g_group, "Operator_Add", [](){
        SchemeInt32 first = 1;
        SchemeInt32 second = 2;
        SchemeFloat third = 3.1415f;
        SchemeFloat fourth = 3.6f;

        // Add SchemeInt32 and ezInt32 (signed int)
        auto result = first + 1;
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first + 1) == 2, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator + must not have sideeffects!");

        // Add SchemeInt32 and SchemeInt32
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first + second) == 3, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(second) == 2, "Operator + must not have sideeffects!");

        // Add SchemeInt32 and SchemeFloat
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first + third) == 4, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeFloat::Number_t(third) == 3.1415f, "Operator + must not have sideeffects!");

        CUT_ASSERT.isTrue(SchemeInt32::Number_t(second + fourth) == 5, "Failed to properly add a SchemeFloat to a SchemeInt32!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(second) == 2, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeFloat::Number_t(fourth) == 3.6f, "Operator + must not have sideeffects!");
    });

    UnitTest g_test6(g_group, "Operator_AddAndAssign", [](){
        // Assign another number
        {
            SchemeInt32 first = 1;
            first += 1;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 2, "Cannot add and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInt32 first = 1;
            SchemeInt32 second = 2;

            first += second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 3, "Cannot add another SchemeInt32!");
        }

        // Assign another scheme number of different type
        {
            SchemeInt32 first = 1;
            SchemeFloat second = 3.1415f;

            first += second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 4, "Cannot add and assign another scheme number of different type!");
        }
    });

    UnitTest g_test7(g_group, "Operator_Subtract", [](){
        SchemeInt32 first = 1;
        SchemeInt32 second = 2;
        SchemeFloat third = 3.1415f;

        // Add SchemeInt32 and ezInt32 (signed int)
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) - 1 == 0, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator - must not have side effects!");

        // Add SchemeInt32 and SchemeInt32
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) - second == -1, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator - must not have side effects!");

        // Add SchemeInt32 and SchemeFloat
        auto result = first - third;
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first - third) == -2, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 1, "Operator - must not have side effects!");
    });

    UnitTest g_test8(g_group, "Operator_SubtractAndAssign", [](){
        // Assign another number
        {
            SchemeInt32 first = 1;
            first -= 1;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 0, "Cannot subtract and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInt32 first = 1;
            SchemeInt32 second = 2;

            first -= second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == -1, "Cannot subtract another SchemeInt32!");
        }

        // Assign another scheme number of different type
        {
            SchemeInt32 first = 1;
            SchemeFloat second = 3.1415f;

            first -= second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == -2, "Cannot subtract and assign another scheme number of different type!");
        }
    });

    UnitTest g_test9(g_group, "Operator_Multiply", [](){
        SchemeInt32 first = 2;
        SchemeInt32 second = 3;
        SchemeFloat third = 4.1415f;
        SchemeFloat fourth = 3.6f;

        // Add SchemeInt32 and ezInt32 (signed int)
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first * 1) == 2, "Failed to multiply with int!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 2, "Operator * must not have sideeffects!");

        // Add SchemeInt32 and SchemeInt32
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first * second) == 6, "Failed to multiply with SchemeInt32!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 2, "Operator * must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(second) == 3, "Operator * must not have sideeffects!");

        // Add SchemeInt32 and SchemeFloat
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first * third) == 8, "Failed to multiply SchemeInt32 with SchemeFloat!");
        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 2, "Operator * must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeFloat::Number_t(third) == 4.1415f, "Operator * must not have sideeffects!");

        CUT_ASSERT.isTrue(SchemeInt32::Number_t(first * fourth) == 7, "Failed to multiply SchemeInt32 with SchemeFloat!");
    });

    UnitTest g_test10(g_group, "Operator_MultiplyAndAssign", [](){
        // Assign another number
        {
            SchemeInt32 first = 2;
            first *= 2;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 4, "Cannot multiply with and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInt32 first = 2;
            SchemeInt32 second = 3;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 6, "Cannot add another SchemeInt32!");
        }

        // Assign another scheme number of different type
        {
            SchemeInt32 first = 2;
            SchemeFloat second = 3.1415f;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 6, "Cannot add and assign another scheme number of different type!");
        }
        {
            SchemeInt32 first = 2;
            SchemeFloat second = 3.6f;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInt32::Number_t(first) == 7, "Cannot add and assign a SchemeFloat or SchemeFloat without truncation!");
        }
    });

}

#pragma warning( pop )
