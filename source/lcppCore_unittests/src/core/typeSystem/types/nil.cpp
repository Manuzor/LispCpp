#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeNilTests)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::AreEqual(SCHEME_NIL.type(), TypeInfo<SchemeNil>::type(), L"SCHEME_NIL.type() returns an incorrect type!");
            Assert::IsTrue(SCHEME_NIL.is(TypeInfo<SchemeNil>::type()), L"SCHEME_NIL.is(...) is not working!");
        }

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(SCHEME_NIL, SCHEME_NIL, L"Scheme nil must equal itself!");
            Assert::AreSame(SCHEME_NIL, SCHEME_NIL, L"Something is seriously b0rken!");

            Assert::AreNotEqual<SchemeObject>(SCHEME_NIL, SCHEME_VOID, L"Wrong result for equality operator!");

            SchemeNil nil;
            Assert::AreEqual(nil, SCHEME_NIL, L"SCHEME_NIL must equal a locally constructed SchemeNil object!");
            Assert::AreEqual(SCHEME_NIL, nil, L"SCHEME_NIL must equal a locally constructed SchemeNil object!");
        }

        TEST_METHOD(ToString)
        {
            SchemeNil nil;
            Assert::IsTrue(SCHEME_NIL.toString().IsEqual("()"), L"SCHEME_NIL.toString() should return \"()\"!");
            Assert::IsTrue(SCHEME_NIL.toString().IsEqual("()"), L"Locally constructed SchemeNil instance nil.toString() should return \"()\"!");
        }
    };

}}
