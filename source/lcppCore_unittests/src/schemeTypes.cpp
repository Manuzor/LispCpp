#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp
{
    TEST_MODULE_INITIALIZE(SchemeTypeTests_Initialize)
    {
        ezStartup::StartupCore();
    }

    TEST_MODULE_CLEANUP(SchemeTypeTests_Cleanup)
    {
        ezStartup::ShutdownBase();
    }

    TEST_CLASS(Test_SchemeType)
    {
    public:
        TEST_METHOD(of)
        {
            Assert::IsTrue(SchemeType::of(SCHEME_VOID) == SchemeType::Void, L"SchemeType::of returns the type for SCHEME_VOID!");
            Assert::IsTrue(SchemeType::of(SCHEME_NIL) == SchemeType::Nil, L"SchemeType::of returns the type for SCHEME_NIL!");
            Assert::IsTrue(SchemeType::of(SCHEME_TRUE) == SchemeType::Bool, L"SchemeType::of returns the type for SCHEME_TRUE!");
            Assert::IsTrue(SchemeType::of(SCHEME_FALSE) == SchemeType::Bool, L"SchemeType::of returns the type for SCHEME_FALSE!");
        }
    };

    TEST_CLASS(Test_SchemeNil)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::IsTrue(SCHEME_NIL.is(SchemeType::Nil), L"Wrong type declaration for SCHEME_NIL!");
        }

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(SCHEME_NIL, SCHEME_NIL, L"Scheme nil must equal itself!");
            Assert::AreSame(SCHEME_NIL, SCHEME_NIL, L"Something is seriously b0rken!");

            Assert::AreNotSame<SchemeObject>(SCHEME_NIL, SCHEME_TRUE, L"Scheme nil and scheme true must not be the same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_NIL, SCHEME_FALSE, L"Scheme nil and scheme false must not be same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_TRUE, SCHEME_NIL, L"Scheme nil and scheme true must not be same instance!");
            Assert::AreNotSame<SchemeObject>(SCHEME_FALSE, SCHEME_NIL, L"Scheme nil and scheme false must not be same instance!");

            Assert::AreEqual<SchemeObject>(SCHEME_NIL, SCHEME_TRUE, L"Wrong result for equality operator!");
            Assert::AreEqual<SchemeObject>(SCHEME_NIL, SCHEME_FALSE, L"Wrong result for equality operator!");
            Assert::AreEqual<SchemeObject>(SCHEME_NIL, SCHEME_VOID, L"Wrong result for equality operator!");
            Assert::AreEqual<SchemeObject>(SCHEME_NIL, SCHEME_TRUE, L"Wrong result for equality operator!");
        }

        TEST_METHOD(toString)
        {
            Assert::IsTrue(ezStringUtils::IsEqual(SCHEME_NIL.toString().GetData(), "()"), L"SCHEME_NIL.toString() should return \"()\"!");
        }
    };

    TEST_CLASS(Test_SchemeBool)
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
            Assert::IsTrue(SCHEME_TRUE, L"Auto conversion of SCHEME_TRUE to 'true' is broken!");
            Assert::IsFalse(SCHEME_FALSE, L"Auto conversion of SCHEME_FALSE to 'false' is broken!");

            Assert::IsTrue(SCHEME_TRUE == SCHEME_TRUE, L"SCHEME_TRUE == SCHEME_TRUE does not evaluate to a boolean true!");
            Assert::IsTrue(SCHEME_FALSE == SCHEME_FALSE, L"SCHEME_FALSE == SCHEME_FALSE does not evaluate to a boolean true!");

            Assert::IsFalse(SCHEME_TRUE != SCHEME_TRUE, L"SCHEME_TRUE != SCHEME_TRUE does not evaluate to a boolean false!");
            Assert::IsFalse(SCHEME_FALSE != SCHEME_FALSE, L"SCHEME_FALSE != SCHEME_FALSE does not evaluate to a boolean alse!");
        }
    };
}