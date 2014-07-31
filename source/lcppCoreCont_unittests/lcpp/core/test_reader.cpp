#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/continuation.h"

namespace lcpp
{
    static Ptr<LispObject> readString(const ezString& content)
    {
        auto pStream = stream::create(content.GetIteratorFront());

        auto pContRead = cont::createTopLevel(&reader::read);
        auto pStack = cont::getStack(pContRead);
        pStack->push(pStream);

        cont::trampoline(pContRead);

        return pStack->get(-1);
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
