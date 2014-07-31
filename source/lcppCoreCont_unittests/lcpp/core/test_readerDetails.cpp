#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"

#include "lcpp/core/runtime.h"

LCPP_TestGroup(ReaderDetails);

LCPP_TestCase(ReaderDetails, skipSeparators)
{
    auto pState = LCPP_pRuntime->getReaderState();

    auto content = ezString("  \t\r\n  \v  \n\n\n\n\r\nhello-world");
    auto pStream = stream::create(content.GetIteratorFront());

    auto count = reader::detail::skipSeparators(pState, pStream);

    CUT_ASSERT.isTrue(count == 16);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'h');
}

LCPP_TestCase(ReaderDetails, skipToFirstNewLine)
{
    auto pState = LCPP_pRuntime->getReaderState();

    auto content = ezString("  ;this is a comment with whitespace \nXello World");
    auto pStream = stream::create(content.GetIteratorFront());

    auto count = reader::detail::skipToFirstNewLine(pState, pStream);

    CUT_ASSERT.isTrue(count == 37);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\n');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'X');
}

LCPP_TestCase(ReaderDetails, advance)
{
    auto pState = LCPP_pRuntime->getReaderState();

    auto content = ezString("a\nb");
    auto pStream = stream::create(content.GetIteratorFront());
    auto count = ezUInt32(0);

    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'a');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pState, pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\n');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pState, pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'b');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pState, pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\0');
    CUT_ASSERT.isFalse(stream::isValid(pStream));
}

LCPP_TestCase(ReaderDetails, isSeparator)
{
    auto pState = LCPP_pRuntime->getReaderState();

    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, ' '));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\t'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\r'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\n'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\v'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\f'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator(pState, '\a'));

    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, '\b'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, 'a'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, 'A'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, 'z'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, 'Z'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, '0'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator(pState, '9'));
}

LCPP_TestCase(ReaderDetails, isNewLine)
{
    auto pState = LCPP_pRuntime->getReaderState();

    CUT_ASSERT.isTrue(reader::detail::isNewLine(pState, '\n'));
    CUT_ASSERT.isFalse(reader::detail::isNewLine(pState, '\r'));
}

LCPP_TestCase(ReaderDetails, isSymbolDelimiter)
{
    auto pState = LCPP_pRuntime->getReaderState();

    CUT_ASSERT.isTrue(reader::detail::isSymbolDelimiter(pState, '('));
    CUT_ASSERT.isTrue(reader::detail::isSymbolDelimiter(pState, ')'));
    CUT_ASSERT.isFalse(reader::detail::isSymbolDelimiter(pState, 'a'));
}

LCPP_TestCase(ReaderDetails, isCommentDelimiter)
{
    auto pState = LCPP_pRuntime->getReaderState();

    CUT_ASSERT.isTrue(reader::detail::isCommentDelimiter(pState, ';'));
    CUT_ASSERT.isFalse(reader::detail::isCommentDelimiter(pState, 'a'));
}
