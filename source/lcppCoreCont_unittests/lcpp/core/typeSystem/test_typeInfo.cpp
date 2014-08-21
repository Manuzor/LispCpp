#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"

LCPP_TestGroup(MetaInfo);

LCPP_TestCase(MetaInfo, Basics)
{
    {
        auto info = MetaInfo();
        info.setType(Type::True);

        CUT_ASSERT.isTrue(info.getType() == Type::True);
        CUT_ASSERT.isTrue(ezStringUtils::IsEqual(info.getPrettyName(), info.getType().toString()));
    }

    {
        auto info = MetaInfo();
        info.setType(Type::True);
        info.setPrettyName("true-type");

        CUT_ASSERT.isTrue(info.getType() == Type::True);
        CUT_ASSERT.isTrue(ezStringUtils::IsEqual(info.getPrettyName(), "true-type"));
    }
}
