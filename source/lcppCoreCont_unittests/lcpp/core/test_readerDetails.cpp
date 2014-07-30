#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"

LCPP_TestGroup(ReaderDetails);

LCPP_TestCase(ReaderDetails, skipSeparators)
{
    auto content = ezString("  \t\r\n  \v  \n\n\n\n\r\nhello-world");
    auto pStream = stream::create(content.GetIteratorFront());

    auto count = reader::detail::skipSeparators(pStream);

    CUT_ASSERT.isTrue(count == 16);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'h');
}

LCPP_TestCase(ReaderDetails, skipToFirstNewLine)
{
    auto content = ezString("  ;this is a comment with whitespace \nXello World");
    auto pStream = stream::create(content.GetIteratorFront());

    auto count = reader::detail::skipToFirstNewLine(pStream);

    CUT_ASSERT.isTrue(count == 37);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\n');
    CUT_ASSERT.isTrue(stream::next(pStream) == 'X');
}

LCPP_TestCase(ReaderDetails, advance)
{
    auto content = ezString("a\nb");
    auto pStream = stream::create(content.GetIteratorFront());
    auto count = ezUInt32(0);

    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'a');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\n');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == 'b');
    CUT_ASSERT.isTrue(stream::isValid(pStream));

    count = reader::detail::advance(pStream);

    CUT_ASSERT.isTrue(count == 1);
    CUT_ASSERT.isTrue(stream::getCharacter(pStream) == '\0');
    CUT_ASSERT.isFalse(stream::isValid(pStream));
}

LCPP_TestCase(ReaderDetails, isSeparator)
{
    CUT_ASSERT.isTrue(reader::detail::isSeparator(' '));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\t'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\r'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\n'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\v'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\f'));
    CUT_ASSERT.isTrue(reader::detail::isSeparator('\a'));

    CUT_ASSERT.isFalse(reader::detail::isSeparator('\b'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('a'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('A'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('z'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('Z'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('0'));
    CUT_ASSERT.isFalse(reader::detail::isSeparator('9'));
}

LCPP_TestCase(ReaderDetails, isNewLine)
{
    CUT_ASSERT.isTrue(reader::detail::isNewLine('\n'));
    CUT_ASSERT.isFalse(reader::detail::isNewLine('\r'));
}

LCPP_TestCase(ReaderDetails, isSymbolDelimiter)
{
    CUT_ASSERT.isTrue(reader::detail::isSymbolDelimiter('('));
    CUT_ASSERT.isTrue(reader::detail::isSymbolDelimiter(')'));
    CUT_ASSERT.isFalse(reader::detail::isSymbolDelimiter('a'));
}

LCPP_TestCase(ReaderDetails, isCommentDelimiter)
{
    CUT_ASSERT.isTrue(reader::detail::isCommentDelimiter(';'));
    CUT_ASSERT.isFalse(reader::detail::isCommentDelimiter('a'));
}
