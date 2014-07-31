#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/continuation.h"

namespace lcpp
{
    static Ptr<LispObject> readStream(Ptr<LispObject> pStream)
    {
        auto pContMain = cont::createTopLevel();
        auto pMainStack = cont::getStack(pContMain);
        
        auto pContRead = cont::create(pContMain, &reader::read);
        cont::getStack(pContRead)->push(pStream);

        cont::trampoline(pContRead);

        return pMainStack->get(0);
    }

    static Ptr<LispObject> readString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        return readStream(pStream);
    }
}

LCPP_TestGroup(Reader);

LCPP_TestCase(Reader, ReadEmptyOrWhitespaceString)
{
    auto pObject = Ptr<LispObject>();

    pObject = readString("");
    CUT_ASSERT.isTrue(isVoid(pObject));

    pObject = readString("    \n\t\r \t\t\n\r\r\n\r\n   \v\v  \n \t");
    CUT_ASSERT.isTrue(isVoid(pObject));
}

LCPP_TestCase(Reader, Atoms)
{
    {
        auto pInteger = readString("42");
        CUT_ASSERT.isTrue(number::getInteger(pInteger) == 42);
    }

    {
        auto pFloat = readString("3.1415");
        CUT_ASSERT.isTrue(number::getFloat(pFloat) == 3.1415);
    }

    {
        auto pSymbol = readString("abc");
        CUT_ASSERT.isTrue(symbol::getValue(pSymbol).IsEqual("abc"));
    }
}

LCPP_TestCase(Reader, State)
{
    auto content = ezString("   123\r\n abc\n \n");
    auto pStream = stream::create(content.GetIteratorFront());
    auto pState = reader::getState();
    auto& cursorPosition = pState->m_syntaxCheckResult.m_cursor.getPosition();

    auto pResult = Ptr<LispObject>();

    //////////////////////////////////////////////////////////////////////////

    CUT_ASSERT.isTrue(cursorPosition.m_streamIndex == 0);
    CUT_ASSERT.isTrue(cursorPosition.m_line == 0);
    CUT_ASSERT.isTrue(cursorPosition.m_column == 0);

    pResult = readStream(pStream);

    CUT_ASSERT.isTrue(number::getInteger(pResult) == 123);
    CUT_ASSERT.isTrue(cursorPosition.m_streamIndex == 6);
    CUT_ASSERT.isTrue(cursorPosition.m_line == 0);
    CUT_ASSERT.isTrue(cursorPosition.m_column == 6);

    pResult = readStream(pStream);

    CUT_ASSERT.isTrue(symbol::getValue(pResult).IsEqual("abc"));
    CUT_ASSERT.isTrue(cursorPosition.m_streamIndex == 12);
    CUT_ASSERT.isTrue(cursorPosition.m_line == 1);
    CUT_ASSERT.isTrue(cursorPosition.m_column == 4);

    pResult = readStream(pStream);

    CUT_ASSERT.isTrue(isVoid(pResult));
    CUT_ASSERT.isTrue(cursorPosition.m_streamIndex == 15);
    CUT_ASSERT.isTrue(cursorPosition.m_line == 3);
    CUT_ASSERT.isTrue(cursorPosition.m_column == 0);
}
