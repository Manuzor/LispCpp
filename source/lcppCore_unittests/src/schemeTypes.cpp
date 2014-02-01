#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp
{
    TEST_CLASS(Test_SchemeNil)
    {
    public:

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(g_nil, g_nil, _T("Scheme nil must equal itself!"));
            Assert::AreSame(g_nil, g_nil, _T("Something is seriously b0rken!"));

            Assert::AreNotSame<SchemeObject>(g_nil, g_true, _T("Scheme nil and scheme true must not be the same instance!"));
            Assert::AreNotSame<SchemeObject>(g_nil, g_false, _T("Scheme nil and scheme false must not be same instance!"));
            Assert::AreNotSame<SchemeObject>(g_true, g_nil, _T("Scheme nil and scheme true must not be same instance!"));
            Assert::AreNotSame<SchemeObject>(g_false, g_nil, _T("Scheme nil and scheme false must not be same instance!"));
        }

        TEST_METHOD(toString)
        {
            Assert::IsTrue(g_nil.toString() == _T("()"), _T("g_nil.toString() should return \"()\"!"));
        }
    };

    TEST_CLASS(Test_SchemeBool)
    {
    public:

        TEST_METHOD(Equality)
        {
            Assert::AreEqual(g_true, g_true, _T("Scheme true must equal itself!"));
            Assert::AreSame(g_true, g_true, _T("Something is seriously b0rken!"));

            Assert::AreEqual(g_false, g_false, _T("Scheme false must equal itself!"));
            Assert::AreSame(g_false, g_false, _T("Something is seriously b0rken!"));

            Assert::AreNotEqual(g_true, g_false, _T("Scheme true cannot equal scheme false!"));
            Assert::AreNotEqual(g_false, g_true, _T("Scheme true cannot equal scheme false!"));
            Assert::AreNotSame(g_true, g_false, _T("Scheme true and scheme false cannot be the exact same instance!"));
        }

        TEST_METHOD(ConversionToBool)
        {
            Assert::IsTrue(g_true, _T("Auto conversion of g_true to 'true' is broken!"));
            Assert::IsFalse(g_false, _T("Auto conversion of g_false to 'false' is broken!"));

            Assert::IsTrue(g_true == g_true, _T("g_true == g_true does not evaluate to a boolean true!"));
            Assert::IsTrue(g_false == g_false, _T("g_false == g_false does not evaluate to a boolean true!"));

            Assert::IsFalse(g_true != g_true, _T("g_true != g_true does not evaluate to a boolean false!"));
            Assert::IsFalse(g_false != g_false, _T("g_false != g_false does not evaluate to a boolean alse!"));
        }
    };
}