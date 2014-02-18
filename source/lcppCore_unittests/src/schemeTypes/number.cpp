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
                SchemeInt32 intMax = 0xFFFF;
                ezString str = intMax.toString();
                Assert::IsTrue(str.IsEqual("65535"), L"SchemeInt32::toString is broken!");
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
                SchemeUInt32 uintMax = 0xFFFFFFFF;
                ezString str = uintMax.toString();
                Assert::IsTrue(str.IsEqual("4294967295"), L"SchemeInt32::toString is broken!");
            }

            // SchemeFloat
            {
                SchemeFloat float1 = 1.0f;
                ezString str = float1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeFloat::toString is broken!");

                //TODO: For some reason, the following line does not compile. Find out why!
                //Assert::IsTrue(float1.toString("%3.f").IsEqual("1.000000"), L"SchemeNumber_t<float>::toString is broken!");
            }
            {
                SchemeFloat float1 = 1.1f;
                ezString str = float1.toString();
                Assert::IsTrue(str.IsEqual("1.1"), L"SchemeFloat::toString is broken!");
            }
        }
    };

}}
