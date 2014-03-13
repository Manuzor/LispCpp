#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {


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

            Assert::AreEqual(SCHEME_FALSE, SCHEME_FALSE, L"Scheme false must equal itself!");

            Assert::AreNotEqual(SCHEME_TRUE, SCHEME_FALSE, L"Scheme true cannot equal scheme false!");
            Assert::AreNotEqual(SCHEME_FALSE, SCHEME_TRUE, L"Scheme true cannot equal scheme false!");
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

}}
