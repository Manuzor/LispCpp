#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(TypeTests)
    {
    public:
        TEST_METHOD(type)
        {
            Assert::AreEqual(SCHEME_VOID.type(), TypeInfo<SchemeVoid>::type(), L"SCHEME_VOID.type() did not return Type::Void!");
            Assert::IsTrue(SCHEME_VOID.is(TypeInfo<SchemeVoid>::type()), L"SCHEME_VOID.is() does not return true for argument Type::Void!");
            Assert::AreEqual(SCHEME_TRUE.type(), TypeInfo<SchemeBool>::type(), L"SCHEME_TRUE.type() did not return Type::Bool!");
            Assert::IsTrue(SCHEME_TRUE.is(TypeInfo<SchemeBool>::type()), L"SCHEME_TRUE.is() does not return true for argument Type::Bool!");
            Assert::AreEqual(SCHEME_FALSE.type(), TypeInfo<SchemeBool>::type(), L"SCHEME_FALSE.type() did not return Type::Bool!");
            Assert::IsTrue(SCHEME_FALSE.is(TypeInfo<SchemeBool>::type()), L"SCHEME_FALSE.is() does not return true for argument Type::Bool!");
        }
    };

}}
