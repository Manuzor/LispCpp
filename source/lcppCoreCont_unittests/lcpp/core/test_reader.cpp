#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"

namespace lcpp
{
    static Ptr<LispObject> readString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        return reader::read(pStream);
    }
}

LCPP_TestGroup(Reader);

LCPP_TestCase(Reader, Basics)
{
    auto pObject = readString("42");

    CUT_ASSERT.isTrue(number::getInteger(pObject) == 42);
}
