#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/functionUtils/signature.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"

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

LCPP_TestCase(Reader, StreamPosition)
{
    // Symbols
    {
        auto content = ezString("abc def");
        auto pStream = stream::create(content.GetIteratorFront());
        auto pSymbol = Ptr<LispObject>();

        pSymbol = readStream(pStream);
        CUT_ASSERT.isTrue(symbol::getValue(pSymbol).IsEqual("abc"));
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == 3);

        pSymbol = readStream(pStream);
        CUT_ASSERT.isTrue(symbol::getValue(pSymbol).IsEqual("def"));
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == stream::EndOfStream);
    }

    // Numbers
    {
        auto content = ezString("   1   2      3.4   5.6");
        auto pStream = stream::create(content.GetIteratorFront());
        auto pNumber = Ptr<LispObject>();

        pNumber = readStream(pStream);
        CUT_ASSERT.isTrue(number::getInteger(pNumber) == 1);
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == 4);

        pNumber = readStream(pStream);
        CUT_ASSERT.isTrue(number::getInteger(pNumber) == 2);
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == 8);

        pNumber = readStream(pStream);
        CUT_ASSERT.isTrue(number::getFloat(pNumber) == 3.4);
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == 17);

        pNumber = readStream(pStream);
        CUT_ASSERT.isTrue(number::getFloat(pNumber) == 5.6);
        CUT_ASSERT.isTrue(stream::getPosition(pStream) == stream::EndOfStream);

    }
}

LCPP_TestCase(Reader, String)
{
    auto content = ezString("   \"hello world\"abc def");
    auto pStream = stream::create(content.GetIteratorFront());
    auto pResult = readStream(pStream);

    CUT_ASSERT.throwsNothing([&]{ typeCheck(pResult, Type::String); });
    CUT_ASSERT.isTrue(str::getValue(pResult).IsEqual("hello world"));

    auto streamPosition = stream::getPosition(pStream);
    CUT_ASSERT.isTrue(streamPosition == 16);
}

LCPP_TestCase(Reader, List)
{
    {
        auto pNil = readString("    (    )   ");

        CUT_ASSERT.isTrue(isNil(pNil));
    }

    {
        auto pCons = readString("   (      x  )   ");
        auto pCar = cons::getCar(pCons);
        auto pCdr = cons::getCdr(pCons);

        CUT_ASSERT.isTrue(symbol::getValue(pCar).IsEqual("x"));
        CUT_ASSERT.isTrue(isNil(pCdr));
    }

    {
        auto pCar = LCPP_pNil;
        auto pCons = readString("   (  42  x   3.1415    )   ");

        pCar = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);
        CUT_ASSERT.isTrue(number::getInteger(pCar) == 42);

        pCar = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);
        CUT_ASSERT.isTrue(symbol::getValue(pCar).IsEqual("x"));

        pCar = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);
        CUT_ASSERT.isTrue(number::getFloat(pCar) == 3.1415);

        CUT_ASSERT.isTrue(isNil(pCons));
    }

    {
        auto pCons = readString("(lambda()42 1337)");

        auto pSyntax_lambda = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);

        CUT_ASSERT.isTrue(object::isType(pSyntax_lambda, Type::Syntax));

        auto pNil = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);

        CUT_ASSERT.isTrue(isNil(pNil));

        auto pInteger_42 = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);

        CUT_ASSERT.isTrue(number::getInteger(pInteger_42) == 42);

        auto pInteger_1337 = cons::getCar(pCons);
        pCons = cons::getCdr(pCons);

        CUT_ASSERT.isTrue(number::getInteger(pInteger_1337) == 1337);
    }
}

LCPP_TestCase(Reader, State)
{
    auto pState = LCPP_test_pRuntimeState->getReaderState();

    auto content = ezString("   123\r\n abc\n \n");
    auto pStream = stream::create(content.GetIteratorFront());
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


LCPP_TestCase(Reader, Quote)
{
    auto pCar = LCPP_pNil;
    auto pCons = readString("  (   quote       x  )");

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(syntax::builtin::getFunction(pCar) == &syntax::builtin::quote);

    pCar = cons::getCar(pCons);
    pCons = cons::getCdr(pCons);
    CUT_ASSERT.isTrue(symbol::getValue(pCar).IsEqual("x"));
    CUT_ASSERT.isTrue(isNil(pCons));

}

namespace lcpp
{
    static Ptr<LispObject> testCharacterMacro_finalize(Ptr<LispObject> pCont);
    static Ptr<LispObject> testCharacterMacro(Ptr<LispObject> pCont);

    static Ptr<LispObject> testCharacterMacro(Ptr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        auto pStack = cont::getStack(pCont);
        auto pStream = pStack->get(1);
        typeCheck(pStream, Type::Stream);

        reader::detail::advance(cont::getRuntimeState(pCont)->getReaderState(), pStream);

        // Just read normally from here.
        cont::setFunction(pCont, &testCharacterMacro_finalize);
        LCPP_cont_call(pCont, &reader::read, pStream);
    }

    static Ptr<LispObject> testCharacterMacro_finalize(Ptr<LispObject> pCont)
    {
        LCPP_cont_return(pCont, number::create(1337));
    }
}

LCPP_TestCase(Reader, CharacterMacro)
{
    auto pState = LCPP_test_pRuntimeState;

    reader::addCharacterMacro(pState,
                              symbol::create("/"),
                              lambda::builtin::create(pState->getGlobalEnvironment(), &testCharacterMacro, Signature::create(1)));

    auto content = ezString("/hello-world");
    auto pStream = stream::create(content.GetIteratorFront());
    auto pResult = readStream(pStream);

    CUT_ASSERT.isTrue(number::getInteger(pResult) == 1337);
    CUT_ASSERT.isTrue(stream::getPosition(pStream) == stream::EndOfStream);
}
