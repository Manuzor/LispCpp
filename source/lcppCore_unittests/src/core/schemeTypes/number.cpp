#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeNumberTests)
    {
        TEST_METHOD(Construction)
        {
            union
            {
                ezUInt32 i;
                float f;
            } test;

            // SchemeInt32
            { // 1
                SchemeInt32 int1 = 1;
                Assert::AreEqual(int1.value(), 1);
            }
            { // 2
                SchemeInt32 int2(2);
                Assert::AreEqual(int2.value(), 2);
            }
            { // min
                SchemeInt32 intMin(-0xFFFF);
                Assert::IsTrue(intMin.value() < 0);
                Assert::AreEqual(intMin.value(), -0xFFFF);
            }
            { // max
                SchemeInt32 intMax(0xFFFF);
                Assert::IsTrue(intMax.value() > 0);
                Assert::AreEqual(intMax.value(), 0xFFFF);
            }

            // SchemeUInt32
            { // 1
                SchemeUInt32 uint1 = 1;
                Assert::AreEqual(uint1.value(), 1U);
            }
            { // 2
                SchemeUInt32 uint2(2);
                Assert::AreEqual(uint2.value(), 2U);
            }
            { // min
                SchemeUInt32 uintMin(0U);
                Assert::AreEqual(uintMin.value(), 0U);
            }
            { // max
                SchemeUInt32 uintMax(0xFFFFFFFFU);
                Assert::AreEqual(uintMax.value(), 0xFFFFFFFFU);
                Assert::AreEqual(uintMax.value(), static_cast<ezUInt32>(-1));
            }

            // SchemeFloat
            { // 1.0f
                SchemeFloat float1_0 = 1.0f;
                Assert::AreEqual(float1_0.value(), 1.0f);
            }
            { // 2.0f
                SchemeFloat float2_0(2.0f);
                Assert::AreEqual(float2_0.value(), 2.0f);
            }
            { // min
                SchemeFloat floatMin(0.0001f);
                Assert::AreEqual(floatMin.value(), 0.0001f);
            }
            { // max
                SchemeFloat floatMax(1337.0f);
                Assert::AreEqual(floatMax.value(), 1337.0f);
            }
            { // first 255 numbers
                for (test.i = 0; test.i < 0xFF; test.i++)
                {
                    SchemeFloat sf = test.f;
                    Assert::AreEqual(sf.value(), test.f);
                }
            }
        }

        TEST_METHOD(ToString)
        {
            // SchemeInt32
            {
                SchemeInt32 int1 = 1;
                ezString str = int1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeInt32::toString is broken!");
            }
            {
                SchemeInt32 int1337 = 1337;
                ezString str = int1337.toString();
                Assert::IsTrue(str.IsEqual("1337"), L"SchemeInt32::toString is broken!");
            }
            {
                SchemeInt32 intMax = std::numeric_limits<SchemeInt32::number_t>::max();
                ezString str = intMax.toString();
                Assert::IsTrue(str.IsEqual("2147483647"), L"SchemeInt32::toString is broken!");
            }
            {
                SchemeInt32 intMin = std::numeric_limits<SchemeInt32::number_t>::min();
                ezString str = intMin.toString();
                Assert::IsTrue(str.IsEqual("-2147483648"), L"SchemeInt32::toString is broken!");
            }

            // SchemeUInt32
            {
                SchemeUInt32 uint1 = 1;
                ezString str = uint1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeUInt32::toString is broken!");
            }
            {
                SchemeUInt32 uint1337 = 1337;
                ezString str = uint1337.toString();
                Assert::IsTrue(str.IsEqual("1337"), L"SchemeInt32::toString is broken!");
            }
            {
                SchemeUInt32 uintMax = std::numeric_limits<SchemeUInt32::number_t>::max();
                ezString str = uintMax.toString();
                Assert::IsTrue(str.IsEqual("4294967295"), L"SchemeInt32::toString is broken!");
            }
            {
                SchemeUInt32 uintMin = std::numeric_limits<SchemeUInt32::number_t>::min();
                ezString str = uintMin.toString();
                Assert::IsTrue(str.IsEqual("0"), L"SchemeInt32::toString is broken!");
            }

            // SchemeFloat
            {
                SchemeFloat float1 = 1.0f;
                ezString str = float1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeFloat::toString is broken!");
            }
            {
                SchemeFloat float1 = 1.1f;
                ezString str = float1.toString();
                Assert::IsTrue(str.IsEqual("1.1"), L"SchemeFloat::toString is broken!");
            }
        }

        TEST_METHOD(Conversion)
        {
            SchemeInt32 theInt = 42;
            Assert::AreEqual(ezInt32(theInt), ezInt32(42), L"Explicit conversion does not work!");
            Assert::AreEqual<ezInt32>(theInt, 42, L"Implicit conversion does not work!");
        }

        TEST_METHOD(Operator_Assign)
        {
            SchemeInt32 first = 1;
            SchemeInt32 second = 2;
            SchemeDouble third = 3.1415;

            first = 3;
            Assert::AreEqual<SchemeInt32::number_t>(first, 3, L"Assignment operator of SchemeNumber_t broken!");

            first = 7.123;
            Assert::AreEqual<SchemeInt32::number_t>(first, 7, L"Failed to assign a float to a SchemeInt32!");

            first = second;
            Assert::AreEqual(first, second, L"Failed to assign a SchemeInt32 to a SchemeInt32!");

            first = third;
            Assert::AreEqual<SchemeInt32::number_t>(first, third, L"Failed to assign a SchemeDouble to a SchemeInt32!");
        }

        TEST_METHOD(Operator_Add)
        {
            SchemeInt32 first = 1;
            SchemeInt32 second = 2;
            SchemeDouble third = 3.1415;
            SchemeFloat fourth = 3.6f;

            // Add SchemeInt32 and ezInt32 (signed int)
            Assert::AreEqual<SchemeInt32::number_t>(first + 1, 2, L"Failed to add int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator + must not have sideeffects!");

            // Add SchemeInt32 and SchemeInt32
            Assert::AreEqual<SchemeInt32::number_t>(first + second, 3, L"Failed to add int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator + must not have sideeffects!");
            Assert::AreEqual<SchemeInt32::number_t>(second, 2, L"Operator + must not have sideeffects!");

            // Add SchemeInt32 and SchemeDouble
            Assert::AreEqual<SchemeInt32::number_t>(first + third, 4, L"Failed to add int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator + must not have sideeffects!");
            Assert::AreEqual<SchemeDouble::number_t>(third, 3.1415, L"Operator + must not have sideeffects!");

            Assert::AreEqual<SchemeInt32::number_t>(second + fourth, 5, L"Failed to properly add a SchemeFloat to a SchemeInt32!");
            Assert::AreEqual<SchemeInt32::number_t>(second, 2, L"Operator + must not have sideeffects!");
            Assert::AreEqual<SchemeFloat::number_t>(fourth, 3.6f, L"Operator + must not have sideeffects!");
        }

        TEST_METHOD(Operator_AddAndAssign)
        {
            // Assign another number
            {
                SchemeInt32 first = 1;
                first += 1;
                Assert::AreEqual<SchemeInt32::number_t>(first, 2, L"Cannot add and assign int!");
            }

            // Assign another scheme number of same type
            {
                SchemeInt32 first = 1;
                SchemeInt32 second = 2;

                first += second;
                Assert::AreEqual<SchemeInt32::number_t>(first, 3, L"Cannot add another SchemeInt32!");
            }

            // Assign another scheme number of different type
            {
                SchemeInt32 first = 1;
                SchemeDouble second = 3.1415;

                first += second;
                Assert::AreEqual<SchemeInt32::number_t>(first, 4, L"Cannot add and assign another scheme number of different type!");
            }
        }

        TEST_METHOD(Operator_Subtract)
        {
            SchemeInt32 first = 1;
            SchemeInt32 second = 2;
            SchemeDouble third = 3.1415;

            // Add SchemeInt32 and ezInt32 (signed int)
            Assert::AreEqual<SchemeInt32::number_t>(first - 1, 0, L"Failed to subtract int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator - must not have sideeffects!");

            // Add SchemeInt32 and SchemeInt32
            Assert::AreEqual<SchemeInt32::number_t>(first - second, -1, L"Failed to subtract int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator - must not have sideeffects!");

            // Add SchemeInt32 and SchemeDouble
            Assert::AreEqual<SchemeInt32::number_t>(first - third, -2, L"Failed to subtract int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 1, L"Operator - must not have sideeffects!");
        }

        TEST_METHOD(Operator_SubtractAndAssign)
        {
            // Assign another number
            {
                SchemeInt32 first = 1;
                first -= 1;
                Assert::AreEqual<SchemeInt32::number_t>(first, 0, L"Cannot subtract and assign int!");
            }

            // Assign another scheme number of same type
            {
                SchemeInt32 first = 1;
                SchemeInt32 second = 2;

                first -= second;
                Assert::AreEqual<SchemeInt32::number_t>(first, -1, L"Cannot subtract another SchemeInt32!");
            }

            // Assign another scheme number of different type
            {
                SchemeInt32 first = 1;
                SchemeDouble second = 3.1415;

                first -= second;
                Assert::AreEqual<SchemeInt32::number_t>(first, -2, L"Cannot subtract and assign another scheme number of different type!");
            }
        }


        TEST_METHOD(Operator_Multiply)
        {
            SchemeInt32 first = 2;
            SchemeInt32 second = 3;
            SchemeDouble third = 4.1415;
            SchemeFloat fourth = 3.6f;

            // Add SchemeInt32 and ezInt32 (signed int)
            Assert::AreEqual<SchemeInt32::number_t>(first * 1, 2, L"Failed to multiply with int!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 2, L"Operator * must not have sideeffects!");

            // Add SchemeInt32 and SchemeInt32
            Assert::AreEqual<SchemeInt32::number_t>(first * second, 6, L"Failed to multiply with SchemeInt32!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 2, L"Operator * must not have sideeffects!");
            Assert::AreEqual<SchemeInt32::number_t>(second, 3, L"Operator * must not have sideeffects!");

            // Add SchemeInt32 and SchemeDouble
            Assert::AreEqual<SchemeInt32::number_t>(first * third, 8, L"Failed to multiply SchemeInt32 with SchemeDouble!");
            Assert::AreEqual<SchemeInt32::number_t>(first, 2, L"Operator * must not have sideeffects!");
            Assert::AreEqual<SchemeDouble::number_t>(third, 4.1415, L"Operator * must not have sideeffects!");

            Assert::AreEqual<SchemeInt32::number_t>(fourth * fourth, 12, L"Failed to multiply SchemeInt32 with SchemeDouble!");
        }

        TEST_METHOD(Operator_MultiplyAndAssign)
        {
            // Assign another number
            {
                SchemeInt32 first = 2;
                first *= 2;
                Assert::AreEqual<SchemeInt32::number_t>(first, 4, L"Cannot multiply with and assign int!");
            }

            // Assign another scheme number of same type
            {
                SchemeInt32 first = 2;
                SchemeInt32 second = 3;

                first *= second;
                Assert::AreEqual<SchemeInt32::number_t>(first, 6, L"Cannot add another SchemeInt32!");
            }

            // Assign another scheme number of different type
            {
                SchemeInt32 first = 2;
                SchemeDouble second = 3.1415;

                first *= second;
                Assert::AreEqual<SchemeInt32::number_t>(first, 6, L"Cannot add and assign another scheme number of different type!");
            }
            {
                SchemeInt32 first = 2;
                SchemeDouble second = 3.6;

                first *= second;
                Assert::AreEqual<SchemeInt32::number_t>(first, 7, L"Cannot add and assign a SchemeFloat or SchemeDouble without truncation!");
            }
        }

        // TODO: More tests for the operators.
    };

}}
