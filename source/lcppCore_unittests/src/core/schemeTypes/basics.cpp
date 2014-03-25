#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {

    TEST_CLASS(SchemeTypeTests)
    {
    public:
        TEST_METHOD(type)
        {
            Assert::AreEqual(SCHEME_VOID.type(), SchemeTypeInfo<SchemeVoid>::type(), L"SCHEME_VOID.type() did not return SchemeType::Void!");
            Assert::IsTrue(SCHEME_VOID.is(SchemeTypeInfo<SchemeVoid>::type()), L"SCHEME_VOID.is() does not return true for argument SchemeType::Void!");
            Assert::AreEqual(SCHEME_TRUE.type(), SchemeTypeInfo<SchemeBool>::type(), L"SCHEME_TRUE.type() did not return SchemeType::Bool!");
            Assert::IsTrue(SCHEME_TRUE.is(SchemeTypeInfo<SchemeBool>::type()), L"SCHEME_TRUE.is() does not return true for argument SchemeType::Bool!");
            Assert::AreEqual(SCHEME_FALSE.type(), SchemeTypeInfo<SchemeBool>::type(), L"SCHEME_FALSE.type() did not return SchemeType::Bool!");
            Assert::IsTrue(SCHEME_FALSE.is(SchemeTypeInfo<SchemeBool>::type()), L"SCHEME_FALSE.is() does not return true for argument SchemeType::Bool!");
        }
    };

}}
