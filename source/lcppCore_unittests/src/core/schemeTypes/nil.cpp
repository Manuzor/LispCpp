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

            Assert::AreNotEqual<SchemeObject>(SCHEME_NIL, SCHEME_VOID, L"Wrong result for equality operator!");
        }

        TEST_METHOD(toString)
        {
            Assert::IsTrue(SCHEME_NIL.toString().IsEqual("()"), L"SCHEME_NIL.toString() should return \"()\"!");
        }
    };

}}
