#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {
    TEST_MODULE_INITIALIZE(SchemeTypeTests_Initialize)
    {
        ezStartup::StartupCore();
    }

    TEST_MODULE_CLEANUP(SchemeTypeTests_Cleanup)
    {
        ezStartup::ShutdownBase();
    }

    TEST_CLASS(SchemeTypeTests)
    {
    public:
        TEST_METHOD(type)
        {
            Assert::AreEqual(SCHEME_VOID.type(), SchemeType::Void, L"SCHEME_VOID.type() did not return SchemeType::Void!");
            Assert::IsTrue(SCHEME_VOID.is(SchemeType::Void), L"SCHEME_VOID.is() does not return true for argument SchemeType::Void!");
            Assert::AreEqual(SCHEME_TRUE.type(), SchemeType::Bool, L"SCHEME_TRUE.type() did not return SchemeType::Bool!");
            Assert::IsTrue(SCHEME_TRUE.is(SchemeType::Bool), L"SCHEME_TRUE.is() does not return true for argument SchemeType::Bool!");
            Assert::AreEqual(SCHEME_FALSE.type(), SchemeType::Bool, L"SCHEME_FALSE.type() did not return SchemeType::Bool!");
            Assert::IsTrue(SCHEME_FALSE.is(SchemeType::Bool), L"SCHEME_FALSE.is() does not return true for argument SchemeType::Bool!");
        }
    };

    TEST_CLASS(SchemeNilTests)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::IsTrue(SCHEME_NIL.type() == SchemeType::Nil, L"SCHEME_NIL.type() does not return SchemeType::Nil!");
            Assert::IsTrue(SCHEME_NIL.is(SchemeType::Nil), L"SCHEME_NIL.is(SchemeType::Nil returns false!");
        }

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(SCHEME_NIL, SCHEME_NIL, L"Scheme nil must equal itself!");
            Assert::AreSame(SCHEME_NIL, SCHEME_NIL, L"Something is seriously b0rken!");

            Assert::AreNotSame<SchemeObject>(SCHEME_NIL, SCHEME_TRUE, L"Scheme nil and scheme true must not be the same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_NIL, SCHEME_FALSE, L"Scheme nil and scheme false must not be same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_TRUE, SCHEME_NIL, L"Scheme nil and scheme true must not be same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_FALSE, SCHEME_NIL, L"Scheme nil and scheme false must not be same instance!");

            Assert::AreNotEqual<SchemeObject>(SCHEME_NIL, SCHEME_TRUE, L"Wrong result for equality operator!");
            Assert::AreNotEqual<SchemeObject>(SCHEME_NIL, SCHEME_FALSE, L"Wrong result for equality operator!");
            Assert::AreNotEqual<SchemeObject>(SCHEME_NIL, SCHEME_VOID, L"Wrong result for equality operator!");
        }

        TEST_METHOD(toString)
        {
            Assert::IsTrue(ezStringUtils::IsEqual(SCHEME_NIL.toString().GetData(), "()"), L"SCHEME_NIL.toString() should return \"()\"!");
        }
    };

    TEST_CLASS(SchemeBoolTests)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::IsTrue(SCHEME_TRUE.is(SchemeType::Bool), L"Wrong type declaration for SCHEME_TRUE!");
            Assert::IsTrue(SCHEME_FALSE.is(SchemeType::Bool), L"Wrong type declaration for SCHEME_FALSE!");
        }

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(SCHEME_TRUE, SCHEME_TRUE, L"Scheme true must equal itself!");
            Assert::AreSame(SCHEME_TRUE, SCHEME_TRUE, L"Something is seriously b0rken!");

            Assert::AreEqual(SCHEME_FALSE, SCHEME_FALSE, L"Scheme false must equal itself!");
            Assert::AreSame(SCHEME_FALSE, SCHEME_FALSE, L"Something is seriously b0rken!");

            Assert::AreNotEqual(SCHEME_TRUE, SCHEME_FALSE, L"Scheme true cannot equal scheme false!");
            Assert::AreNotEqual(SCHEME_FALSE, SCHEME_TRUE, L"Scheme true cannot equal scheme false!");
            Assert::AreNotSame(SCHEME_TRUE, SCHEME_FALSE, L"Scheme true and scheme false cannot be the exact same instance!");
        }

        TEST_METHOD(ConversionToBool)
        {
            bool result = SCHEME_TRUE;
            Assert::IsTrue(SCHEME_TRUE, L"Auto conversion of SCHEME_TRUE to 'true' is broken!");
            Assert::IsFalse(SCHEME_FALSE, L"Auto conversion of SCHEME_FALSE to 'false' is broken!");

            Assert::IsTrue(SCHEME_TRUE == SCHEME_TRUE, L"SCHEME_TRUE == SCHEME_TRUE does not evaluate to a boolean true!");
            Assert::IsTrue(SCHEME_FALSE == SCHEME_FALSE, L"SCHEME_FALSE == SCHEME_FALSE does not evaluate to a boolean true!");

            Assert::IsFalse(SCHEME_TRUE != SCHEME_TRUE, L"SCHEME_TRUE != SCHEME_TRUE does not evaluate to a boolean false!");
            Assert::IsFalse(SCHEME_FALSE != SCHEME_FALSE, L"SCHEME_FALSE != SCHEME_FALSE does not evaluate to a boolean alse!");
        }
    };

    TEST_CLASS(SchemeNumberTests)
    {
        TEST_METHOD(Construction)
        {
            SchemeInt schemeNumber1 = 1;
            SchemeInt schemeNumber2(2);
        }

        TEST_METHOD(ToString)
        {
            // SchemeInt
            {
                SchemeInt int1 = 1;
                ezString str = int1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeInt::toString is broken!");
            }
            {
                SchemeInt int1337 = 1337;
                ezString str = int1337.toString();
                Assert::IsTrue(str.IsEqual("1337"), L"SchemeInt::toString is broken!");
            }
            {
                SchemeInt intMax = 0xFFFF;
                ezString str = intMax.toString();
                Assert::IsTrue(str.IsEqual("65535"), L"SchemeInt::toString is broken!");
            }

            // SchemeUInt
            {
                SchemeUInt uint1 = 1;
                ezString str = uint1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeUInt::toString is broken!");
            }
            {
                SchemeUInt uint1337 = 1337;
                ezString str = uint1337.toString();
                Assert::IsTrue(str.IsEqual("1337"), L"SchemeInt::toString is broken!");
            }
            {
                SchemeUInt uintMax = 0xFFFFFFFF;
                ezString str = uintMax.toString();
                Assert::IsTrue(str.IsEqual("4294967295"), L"SchemeInt::toString is broken!");
            }

            // SchemeFloat
            {
                SchemeFloat float1 = 1.0f;
                ezString str = float1.toString();
                Assert::IsTrue(str.IsEqual("1"), L"SchemeFloat::toString is broken!");

                //TODO: For some reason, the following line does not work. Find out why!
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