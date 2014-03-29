#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeVoidTests)
    {
    public:

        TEST_METHOD(Type)
        {
            Assert::AreEqual(SCHEME_VOID.type(), TypeInfo<SchemeVoid>::type(), L"SCHEME_VOID.type() returns an incorrect type!");
            Assert::IsTrue(SCHEME_VOID.is(TypeInfo<SchemeVoid>::type()), L"SCHEME_VOID.is(...) is not working!");
        }

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(SCHEME_VOID, SCHEME_VOID, L"Scheme nil must equal itself!");
            Assert::AreSame(SCHEME_VOID, SCHEME_VOID, L"Something is seriously b0rken!");

            Assert::AreNotEqual<SchemeObject>(SCHEME_VOID, SCHEME_NIL, L"Wrong result for equality operator!");

            SchemeVoid v;
            Assert::AreEqual(v, SCHEME_VOID, L"SCHEME_VOID must equal a locally constructed SchemeVoid object!");
            Assert::AreEqual(SCHEME_VOID, v, L"SCHEME_VOID must equal a locally constructed SchemeVoid object!");
        }

        TEST_METHOD(ToString)
        {
            SchemeVoid v;
            Assert::IsTrue(SCHEME_VOID.toString().IsEqual("#v"), L"SCHEME_VOID.toString() should return \"#v\"!");
            Assert::IsTrue(SCHEME_VOID.toString().IsEqual("#v"), L"Locally constructed SchemeVoid instance v.toString() should return \"#v\"!");
        }
    };

}}
