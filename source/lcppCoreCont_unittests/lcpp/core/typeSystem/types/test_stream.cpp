#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/nil.h"

LCPP_TestGroup(Stream);

LCPP_TestCase(Stream, Basics)
{
    auto content = ezString("abcdef");
    StackPtr<LispObject> pStream = stream::create(str::create(content.GetData(), content.GetElementCount()));

    CUT_ASSERT.isTrue(pStream);
    CUT_ASSERT.isTrue(object::isType(pStream, Type::Stream));
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

LCPP_TestCase(Stream, getPosition)
{
    auto content = ezString("abcdef");
    StackPtr<LispObject> pStream = stream::create(content);

    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 0);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 1);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 2);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 3);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 4);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == 5);

    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == stream::EndOfStream);

    stream::next(pStream);
    stream::next(pStream);
    stream::next(pStream);
    stream::next(pStream);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == stream::EndOfStream);
}

LCPP_TestCase(Stream, toString)
{
    auto content = ezString("abcdef");
    StackPtr<LispObject> pStream = stream::create(content.GetIteratorFront());
    StackPtr<LispObject> pString = LCPP_pNil;

    pString = object::toString(pStream);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<stream at 0: \"abcdef\">"));

    stream::next(pStream);
    stream::next(pStream);
    pString = object::toString(pStream);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<stream at 2: \"cdef\">"));

    stream::next(pStream);
    stream::next(pStream);
    stream::next(pStream);
    stream::next(pStream);
    pString = object::toString(pStream);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<stream at end>"));
}
