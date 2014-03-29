#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {


    TEST_CLASS(SchemeBoolTests)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::IsTrue(SCHEME_TRUE.is(TypeInfo<SchemeBool>::type()), L"Wrong type declaration for SCHEME_TRUE!");
            Assert::IsTrue(SCHEME_FALSE.is(TypeInfo<SchemeBool>::type()), L"Wrong type declaration for SCHEME_FALSE!");
        }

        TEST_METHOD(Construction)
        {
            SchemeBool t(true);
            SchemeBool f(false);

            SchemeBool tOther(t);
            SchemeBool fOther = false;

            Assert::IsTrue( t.value(),      L"SchemeBool(true) appears to be not working correctly.");
            Assert::IsFalse(f.value(),      L"SchemeBool(false) appears to be not working correctly.");
            Assert::IsTrue( tOther.value(), L"Copy constructor of SchemeBool failed to construct object with the correct value.");

            auto implicitConversion = [](SchemeBool b) -> bool
            {
                Assert::IsTrue(b.value(), L"Implicit conversion failed."); return b;
            };

            Assert::IsTrue(implicitConversion(true), L"Implicit conversion failed.");
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

        TEST_METHOD(ToString)
        {
            Assert::AreEqual("#t", SCHEME_TRUE.toString().GetData(), L"SCHEME_TRUE has the wrong string representation!");
            Assert::AreEqual("#f", SCHEME_FALSE.toString().GetData(), L"SCHEME_FALSE has the wrong string representation!");
        }
    };

}}
