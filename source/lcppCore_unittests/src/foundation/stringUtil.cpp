#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
	UnitTestGroup g_group("StringUtil");

    UnitTest g_test1(g_group, "ToString", [](){
		{
			auto str = toString(1);
			CUT_ASSERT.isTrue(str.IsEqual("1"));
		}
		{
			auto str = toString(1U);
			CUT_ASSERT.isTrue(str.IsEqual("1"));
		}
		{
			auto str = toString(1.0f);
			CUT_ASSERT.isTrue(str.IsEqual("1"));

			str = toString(1.1f);
			CUT_ASSERT.isTrue(str.IsEqual("1.1"));
		}
		{
			auto str = toString(1.0);
			CUT_ASSERT.isTrue(str.IsEqual("1"));

			str = toString(1.1);
			CUT_ASSERT.isTrue(str.IsEqual("1.1"));
		}
	});
}
