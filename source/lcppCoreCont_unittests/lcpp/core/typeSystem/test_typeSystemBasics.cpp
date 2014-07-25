#include "stdafx.h"
#include "lcpp/core/typeSystem/typeInfo.h"
#include "lcpp/core/typeSystem/typeTags.h"

LCPP_TEST_GROUP(TypeSystem);

LCPP_TEST(TypeSystem, Tags)
{
    auto tags = TypeTags(TypeTags::Callable | TypeTags::Nameable);

    CUT_ASSERT.isTrue(tags.isCallable());
    CUT_ASSERT.isTrue(tags.isNameable());

    tags = TypeTags();

    CUT_ASSERT.isFalse(tags.isCallable());
    CUT_ASSERT.isFalse(tags.isNameable());

    tags = TypeTags(TypeTags::Callable);

    CUT_ASSERT.isTrue(tags.isCallable());
    CUT_ASSERT.isFalse(tags.isNameable());
}
