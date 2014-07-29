#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(Stream);

LCPP_TestCase(Stream, Basics)
{
    auto content = ezString("abcdef");
    auto pStream = stream::create(content.GetIteratorFront());

    CUT_ASSERT.isTrue(pStream);
    CUT_ASSERT.isTrue(stream::isValid(pStream));
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'a');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'b');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'c');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'd');
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'd');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'e');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'f');
    CUT_ASSERT.isTrue(stream::next(pStream) == '\0');
    CUT_ASSERT.isFalse(stream::isValid(pStream));
}
