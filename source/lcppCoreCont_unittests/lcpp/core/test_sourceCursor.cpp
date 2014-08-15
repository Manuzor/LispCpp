#include "stdafx.h"
#include "lcpp/core/sourceCursor.h"

LCPP_TestGroup(SourceCursor);

LCPP_TestCase(SourceCursor, Basics)
{
    auto cursor = SourceCursor();
    auto& position = cursor.getPosition();

    CUT_ASSERT.isTrue(position.m_streamIndex == 0);
    CUT_ASSERT.isTrue(position.m_line == 0);
    CUT_ASSERT.isTrue(position.m_column == 0);

    cursor.advance();
    CUT_ASSERT.isTrue(position.m_streamIndex == 1);
    CUT_ASSERT.isTrue(position.m_line == 0);
    CUT_ASSERT.isTrue(position.m_column == 1);

    cursor.advance();
    CUT_ASSERT.isTrue(position.m_streamIndex == 2);
    CUT_ASSERT.isTrue(position.m_line == 0);
    CUT_ASSERT.isTrue(position.m_column == 2);

    cursor.lineBreak();
    CUT_ASSERT.isTrue(position.m_streamIndex == 3);
    CUT_ASSERT.isTrue(position.m_line == 1);
    CUT_ASSERT.isTrue(position.m_column == 0);

    cursor.reset();
    CUT_ASSERT.isTrue(position.m_streamIndex == 0);
    CUT_ASSERT.isTrue(position.m_line == 0);
    CUT_ASSERT.isTrue(position.m_column == 0);

    {
        auto& position = const_cast<const SourceCursor&>(cursor).getPosition();
        //position.m_streamIndex = 123;
        //position.m_line = 123;
        //position.m_column = 123;
    }
}
