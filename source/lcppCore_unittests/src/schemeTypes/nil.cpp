#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

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

}}
