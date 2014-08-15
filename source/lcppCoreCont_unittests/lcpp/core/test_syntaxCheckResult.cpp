#include "stdafx.h"
#include "lcpp/core/syntaxCheckResult.h"

LCPP_TestGroup(SyntaxCheckResult);

LCPP_TestCase(SyntaxCheckResult, Basics)
{
    auto syntax = SyntaxCheckResult();

    CUT_ASSERT.isTrue(syntax.isComplete());
    CUT_ASSERT.isFalse(syntax.m_valid);
    CUT_ASSERT.isTrue(syntax.m_isPureWhitespace);
    CUT_ASSERT.isFalse(syntax.m_hasParenthesis);
}
