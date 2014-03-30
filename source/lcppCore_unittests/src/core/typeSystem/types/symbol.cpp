#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp { namespace unittests {


    TEST_CLASS(SchemeSymbolTests)
    {
    public:

        TEST_METHOD(Type)
        {
            SchemeSymbol s1("a");
            SchemeSymbol s2("b");

            Assert::AreEqual(s1.type(), s2.type(), L"Different instances of scheme symbol do not have the same type!");
        }

        TEST_METHOD(Construction)
        {
            ezString str("Hello World");
            const char* cstr = "Goodbye Cruel World";

            SchemeSymbol s1(str);
            SchemeSymbol s2(cstr);

            Assert::IsTrue(s1.value().IsEqual(str.GetData()), L"Construction from another ezString failed.");
            Assert::IsTrue(s2.value().IsEqual(cstr), L"Construction from a c-string failed.");
        }

        TEST_METHOD(Equality)
        {
            // == with another symbol
            {
                SchemeSymbol s1("Hello World");
                SchemeSymbol s2("Hello World");

                Assert::AreEqual(s1, s2, L"Equality operator for comparing two symbols is not working!");
                Assert::AreEqual(s2, s1, L"Equality operator for comparing two symbols is not commutative!");
            }

            // == with a c-string
            {
                const char* cstr = "Hello World";
                SchemeSymbol s1("Hello World");

                Assert::IsTrue(s1 == cstr, L"Equality operator for comparing a scheme symbol with a c-string is not working!");
                Assert::IsTrue(cstr == s1, L"Equality operator for comparing a scheme symbol with a c-string is not commutative!");
            }

            // == with an ezString
            {
                ezString str = "Hello World";
                SchemeSymbol s1("Hello World");

                Assert::IsTrue(s1 == str, L"Equality operator for comparing a scheme symbol with an ezString is not working!");
                Assert::IsTrue(str == s1, L"Equality operator for comparing a scheme symbol with an ezString is not commutative!");
            }
        }

        TEST_METHOD(ConversionToString)
        {
            SchemeSymbol s1("Hello World");
            ezString str(s1);

            Assert::IsTrue(s1.toString().IsEqual(str.GetData()), L"s.toString should return the same thing as s.value()!");
            Assert::IsTrue(str.IsEqual(s1.value().GetData()), L"s.toString should return the same thing as s.value()!");
        }

        TEST_METHOD(ToString)
        {
            SchemeSymbol s1("Hello World");

            Assert::IsTrue(s1.toString().IsEqual(s1.value().GetData()), L"s.toString() should return the same thing as s.value()!");
        }
    };

}}
