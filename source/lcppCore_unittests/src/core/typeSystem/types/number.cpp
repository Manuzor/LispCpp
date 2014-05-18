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

        // SchemeInteger
        { // 1
            SchemeInteger int1 = 1;
            CUT_ASSERT.isTrue(int1.value() == 1);
        }
        { // 2
            SchemeInteger int2(2);
            CUT_ASSERT.isTrue(int2.value() == 2);
        }
        { // min
            SchemeInteger intMin(-0xFFFF);
            CUT_ASSERT.isTrue(intMin.value() < 0);
            CUT_ASSERT.isTrue(intMin.value() == -0xFFFF);
        }
        { // max
            SchemeInteger intMax(0xFFFF);
            CUT_ASSERT.isTrue(intMax.value() > 0);
            CUT_ASSERT.isTrue(intMax.value() == 0xFFFF);
        }

        // SchemeInteger
        { // 1
            SchemeInteger uint1 = 1;
            CUT_ASSERT.isTrue(uint1.value() == 1U);
        }
        { // 2
            SchemeInteger uint2(2);
            CUT_ASSERT.isTrue(uint2.value() == 2U);
        }
        { // min
            SchemeInteger uintMin(0U);
            CUT_ASSERT.isTrue(uintMin.value() == 0U);
        }
        { // max
            SchemeInteger uintMax(0xFFFFFFFFU);
            CUT_ASSERT.isTrue(uintMax.value() == 0xFFFFFFFFU);
            CUT_ASSERT.isTrue(uintMax.value() == static_cast<ezUInt32>(-1));
        }

        // SchemeNumber
        { // 1.0f
            SchemeNumber float1_0 = 1.0f;
            CUT_ASSERT.isTrue(float1_0.value() == 1.0f);
        }
        { // 2.0f
            SchemeNumber float2_0(2.0f);
            CUT_ASSERT.isTrue(float2_0.value() == 2.0f);
        }
        { // min
            SchemeNumber floatMin(0.0001f);
            CUT_ASSERT.isTrue(floatMin.value() == 0.0001f);
        }
        { // max
            SchemeNumber floatMax(1337.0f);
            CUT_ASSERT.isTrue(floatMax.value() == 1337.0f);
        }
        { // first 255 numbers
            for (test.i = 0; test.i < 0xFF; test.i++)
            {
                SchemeNumber sf = test.f;
                CUT_ASSERT.isTrue(sf.value() == test.f);
            }
        }
    });

    UnitTest g_test2(g_group, "ToString", [](){
        // SchemeInteger
        {
            SchemeInteger int1 = 1;
            ezString str = int1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeInteger::toString is broken!");
        }
        {
            SchemeInteger int1337 = 1337;
            ezString str = int1337.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1337"), "SchemeInteger::toString is broken!");
        }
        {
            SchemeInteger intMax = std::numeric_limits<SchemeInteger::Number_t>::max();
            ezString str = intMax.toString();
            CUT_ASSERT.isTrue(str.IsEqual("9223372036854775807"), "SchemeInteger::toString is broken!");
        }
        {
            SchemeInteger intMin = std::numeric_limits<SchemeInteger::Number_t>::min();
            ezString str = intMin.toString();
            CUT_ASSERT.isTrue(str.IsEqual("-9223372036854775808"), "SchemeInteger::toString is broken!");
        }

        // SchemeNumber
        {
            SchemeNumber number1 = 1.0f;
            ezString str = number1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1"), "SchemeNumber::toString is broken!");
        }
        {
            SchemeNumber number1 = 1.1f;
            ezString str = number1.toString();
            CUT_ASSERT.isTrue(str.IsEqual("1.1"), "SchemeNumber::toString is broken!");
        }
    });

    UnitTest g_test4(g_group, "Operator_Assign", [](){
        SchemeInteger second = 2;
        SchemeNumber third = 3.1415f;

        {
            SchemeInteger first(3);
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 3,
                              "Assignment operator of SchemeNumber_t broken!");
        }
        {
            SchemeInteger first(7.123);
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 7,
                              "Failed to assign a float to a SchemeInteger!");
        }
        {
            SchemeInteger first(second);
            CUT_ASSERT.isTrue(first == second,
                              "Failed to assign a SchemeInteger to a SchemeInteger!");
        }
        {
            SchemeInteger first(third.value());
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == SchemeInteger::Number_t(third.value()),
                              "Failed to assign a SchemeNumber to a SchemeInteger!");
        }
    });

    UnitTest g_test5(g_group, "Operator_Add", [](){
        SchemeInteger first = 1;
        SchemeInteger second = 2;
        SchemeNumber third = 3.1415f;
        SchemeNumber fourth = 3.6f;

        // Add SchemeInteger and ezInt32 (signed int)
        auto result = first + 1;
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() + 1) == 2, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator + must not have sideeffects!");

        // Add SchemeInteger and SchemeInteger
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() + second.value()) == 3, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(second.value()) == 2, "Operator + must not have sideeffects!");

        // Add SchemeInteger and SchemeNumber
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() + third.value()) == 4, "Failed to add int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeNumber::Number_t(third.value()) == 3.1415f, "Operator + must not have sideeffects!");

        CUT_ASSERT.isTrue(SchemeInteger::Number_t(second.value() + fourth.value()) == 5, "Failed to properly add a SchemeNumber to a SchemeInteger!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(second.value()) == 2, "Operator + must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeNumber::Number_t(fourth.value()) == 3.6f, "Operator + must not have sideeffects!");
    });

    UnitTest g_test6(g_group, "Operator_AddAndAssign", [](){
        // Assign another number
        {
            SchemeInteger first = 1;
            first += 1;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 2, "Cannot add and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInteger first = 1;
            SchemeInteger second = 2;

            first += second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 3, "Cannot add another SchemeInteger!");
        }

        // Assign another scheme number of different type
        {
            SchemeInteger first = 1;
            SchemeNumber second = 3.1415f;

            first += second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 4, "Cannot add and assign another scheme number of different type!");
        }
    });

    UnitTest g_test7(g_group, "Operator_Subtract", [](){
        SchemeInteger first = 1;
        SchemeInteger second = 2;
        SchemeNumber third = 3.1415f;

        // Add SchemeInteger and ezInt32 (signed int)
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) - 1 == 0, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator - must not have side effects!");

        // Add SchemeInteger and SchemeInteger
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) - second == -1, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator - must not have side effects!");

        // Add SchemeInteger and SchemeNumber
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() - third.value()) == -2, "Failed to subtract int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 1, "Operator - must not have side effects!");
    });

    UnitTest g_test8(g_group, "Operator_SubtractAndAssign", [](){
        // Assign another number
        {
            SchemeInteger first = 1;
            first -= 1;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 0, "Cannot subtract and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInteger first = 1;
            SchemeInteger second = 2;

            first -= second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == -1, "Cannot subtract another SchemeInteger!");
        }

        // Assign another scheme number of different type
        {
            SchemeInteger first = 1;
            SchemeNumber second = 3.1415f;

            first -= second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == -2, "Cannot subtract and assign another scheme number of different type!");
        }
    });

    UnitTest g_test9(g_group, "Operator_Multiply", [](){
        SchemeInteger first = 2;
        SchemeInteger second = 3;
        SchemeNumber third = 4.1415f;
        SchemeNumber fourth = 3.6f;

        // Add SchemeInteger and ezInt32 (signed int)
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first * 1) == 2, "Failed to multiply with int!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 2, "Operator * must not have sideeffects!");

        // Add SchemeInteger and SchemeInteger
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first * second) == 6, "Failed to multiply with SchemeInteger!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 2, "Operator * must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(second.value()) == 3, "Operator * must not have sideeffects!");

        // Add SchemeInteger and SchemeNumber
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() * third.value()) == 8, "Failed to multiply SchemeInteger with SchemeNumber!");
        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 2, "Operator * must not have sideeffects!");
        CUT_ASSERT.isTrue(SchemeNumber::Number_t(third.value()) == 4.1415f, "Operator * must not have sideeffects!");

        CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value() * fourth.value()) == 7, "Failed to multiply SchemeInteger with SchemeNumber!");
    });

    UnitTest g_test10(g_group, "Operator_MultiplyAndAssign", [](){
        // Assign another number
        {
            SchemeInteger first = 2;
            first *= 2;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 4, "Cannot multiply with and assign int!");
        }

        // Assign another scheme number of same type
        {
            SchemeInteger first = 2;
            SchemeInteger second = 3;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 6, "Cannot add another SchemeInteger!");
        }

        // Assign another scheme number of different type
        {
            SchemeInteger first = 2;
            SchemeNumber second = 3.1415f;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 6, "Cannot add and assign another scheme number of different type!");
        }
        {
            SchemeInteger first = 2;
            SchemeNumber second = 3.6f;

            first *= second;
            CUT_ASSERT.isTrue(SchemeInteger::Number_t(first.value()) == 7, "Cannot add and assign a SchemeNumber or SchemeNumber without truncation!");
        }
    });

}

#pragma warning( pop )
