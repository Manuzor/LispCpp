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
            Assert::AreNotSame<SchemeObject>(g_nil, g_true, _T("Scheme nil cannot be equal to scheme true!"));
        }

        TEST_METHOD(Test2)
        {
            // TODO: Your test code here
            Assert::Fail(_T("Not implemented!"));
        }
    };
}