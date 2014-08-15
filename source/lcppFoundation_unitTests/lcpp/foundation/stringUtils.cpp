#include "stdafx.h"
#include "lcpp/foundation/stringUtils.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("StringUtils");

    UnitTest g_test1(g_group, "Contains", []{
        String str("abcdef");
        CUT_ASSERT.isTrue(contains(str, 'a'), "String does not contain 'x'!");
        CUT_ASSERT.isFalse(contains(str, 'x'), "String does not contain 'x'!");
        CUT_ASSERT.isFalse(contains(str, '\n'), "String does not contain a new-line character!");
    });
}
