#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace lcpp
{		
	TEST_CLASS(SchemeNil)
	{
	public:

		TEST_METHOD(Test1)
		{
			Assert::AreEqual(g_nil, g_nil, _T("Scheme nil must equal itself!"));
		}

		TEST_METHOD(Test2)
		{
			// TODO: Your test code here
			Assert::Fail(_T("Not implemented!"));
		}
	};
}