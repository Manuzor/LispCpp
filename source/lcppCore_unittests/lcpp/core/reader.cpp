#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("ReaderTests");

    UnitTest g_test1(g_group, "IsSeparator", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        // These should be true.
        CUT_ASSERT.isTrue(reader.isSeparator(' '), "Space should be a default separator!");
        CUT_ASSERT.isTrue(reader.isSeparator('\n'), "New-line should be a default separator!");
        CUT_ASSERT.isTrue(reader.isSeparator('\t'), "Tab should be a default separator!");
        CUT_ASSERT.isTrue(reader.isSeparator('\r'), "Carriage-return should be a default separator!");

        // These should be false.
        CUT_ASSERT.isFalse(reader.isSeparator('a'), "'a' should not be a default separator!");
        CUT_ASSERT.isFalse(reader.isSeparator('1'), "'1' should not be a default separator!");
        CUT_ASSERT.isFalse(reader.isSeparator(L'\x123'), "'\\x123' should not be a default separator!");
    });

    UnitTest g_test2(g_group, "SkipSeparators", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();
        String str = "   abc";
        auto iter = str.GetIteratorFront();

        reader.skipSeparators(iter);
        CUT_ASSERT.isTrue(iter.GetCharacter() == 'a', "Unable to skip separators.");
    });

    UnitTest g_test3(g_group, "Read", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            auto object = reader.read("42");

            auto& staticType = LispInteger::typeInfo();
            auto& type = object->type();

            CUT_ASSERT.isTrue(type == staticType, "Type ID mismatch for scheme integer!");
            CUT_ASSERT.isTrue(dynamic_cast<LispInteger*>(object.get()) != nullptr, "Reader did not return a scheme integer instance!");
            CUT_ASSERT.isTrue(object->toString().IsEqual("42"), "Wrong string representation");
        }

        {
            auto pObject = reader.read("3.1415");

            CUT_ASSERT.isTrue(dynamic_cast<LispNumber*>(pObject.get()) != nullptr, "Reader did not return a scheme number instance!");
            CUT_ASSERT.isTrue(pObject->type() == LispNumber::typeInfo(), "Type ID mismatch for scheme number!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("3.1415"), "Wrong string representation");
        }

        {
            auto pObject = reader.read("hello world");

            CUT_ASSERT.isTrue(dynamic_cast<LispSymbol*>(pObject.get()) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(pObject->type() == LispSymbol::typeInfo(), "Type ID mismatch for scheme symbol!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("hello"), "Wrong string representation");
        }

        {
            auto pObject = reader.read("\"hello world\"");

            CUT_ASSERT.isTrue(dynamic_cast<LispString*>(pObject.get()) != nullptr, "Reader did not return a scheme string instance!");
            CUT_ASSERT.isTrue(pObject->type() == LispString::typeInfo(), "Type ID mismatch for scheme string!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("\"hello world\""), "Wrong string representation");
        }
    });

    UnitTest g_test4(g_group, "parseInteger", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String str("123");
            auto i = reader.parseInteger(str.GetIteratorFront());
            CUT_ASSERT.isTrue(i->value() == 123, "Invalid value of parsed integer.");
            CUT_ASSERT.isTrue(i->toString().IsEqual("123"), "Invalid string representation of parsed integer.");
        }

        {
            String str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseInteger(str.GetIteratorFront());
            }, "'qwerty' should not be parsed as integer!");
        }
    });

    UnitTest g_test5(g_group, "parseNumber", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String str("3.1415");
            auto number = reader.parseNumber(str.GetIteratorFront());
            CUT_ASSERT.isTrue(number->value() == 3.1415, "Invalid value of parsed number.");
            CUT_ASSERT.isTrue(number->toString().IsEqual("3.1415"), "Invalid string representation of parsed number.");
        }

        {
            String str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseNumber(str.GetIteratorFront());
            }, "'qwerty' should not be parsed as number!");
        }
    });

    UnitTest g_test6(g_group, "parseSymbol", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String str("qwerty");
            auto symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol->value().IsEqual("qwerty"), "Invalid value of parsed symbol.");
        }

        {
            String str("hello world with spaces");
            auto symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol->value().IsEqual("hello"), "Invalid value of parsed symbol. It should not consume spaces");
        }

        {
            String str("    leadingWhiteSpace");
            auto symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol->value().IsEqual("leadingWhiteSpace"), "Invalid value of parsed symbol.");
        }

        {
            String str("123");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseSymbol(str.GetIteratorFront());
            }, "'123' should not be parsed as symbol!");
        }
    });

    UnitTest g_test7(g_group, "parseString", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String input("\"qwerty\"123");
            auto iter = input.GetIteratorFront();
            auto str = reader.parseString(iter);
            CUT_ASSERT.isTrue(str->value().IsEqual("qwerty"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str->toString().IsEqual("\"qwerty\""), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(iter.GetCharacter() == '1', "Reader did not consume the trailing \" character!");
        }

        {
            String input("\"123\"");
            auto str = reader.parseString(input.GetIteratorFront());
            CUT_ASSERT.isTrue(str->value().IsEqual("123"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str->toString().IsEqual("\"123\""), "Invalid value of parsed string.");
        }

        {
            String input("hello");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseString(input.GetIteratorFront());
            }, "'hello' should not be parsed as string! ('\"hello\"' should)");
        }

        {
            String input("\"hello");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseString(input.GetIteratorFront());
            }, "'\"hello' should not be recognized as a complete string! ('\"hello\"' should)");
        }
    });

    UnitTest g_test8(g_group, "parseList", []()
    {
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String input("hello world 123 42");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&]{
                reader.parseList(input.GetIteratorFront());
            }, "The input string 'hello world 123 42' should not be read as a valid list!");
        }

        {
            String input("()");
            auto consObject = reader.parseList(input.GetIteratorFront());
            CUT_ASSERT.isTrue(consObject == LCPP_NIL, "Invalid result for parsing the list.");
        }

        {
            String input("(theSymbol x 1)");
            String strOutput;
            auto consObject = reader.parseList(input.GetIteratorFront());
            auto cons = consObject.cast<LispCons>();
            CUT_ASSERT.isTrue(!isNil(consObject), "Invalid result for parsing the list. (expected cons, got nil)");
            CUT_ASSERT.isTrue(consObject->toString().IsEqual("(theSymbol x 1)"), "Wrong string representation for read list!");
            auto car = cons->car(); // "theSymbol"
            CUT_ASSERT.isTrue(car->is<LispSymbol>(), "First car is supposed to be the symbol 'theSymbol'.");
            CUT_ASSERT.isTrue(car.cast<LispSymbol>()->value().IsEqual("theSymbol"), "First car is supposed to be the symbol 'theSymbol'.");
            CUT_ASSERT.isTrue(cons->cdr()->is<LispCons>(), "The Cdr is supposed to be a cons!.");
            auto cdr = cons->cdr().cast<LispCons>();
            auto cdr_car = cdr->car(); // "x"
            CUT_ASSERT.isTrue(cdr_car->is<LispSymbol>(), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr_car.cast<LispSymbol>()->value().IsEqual("x"), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr->cdr()->is<LispCons>(), "First car is supposed to be the symbol 'theSymbol'.");
            auto cdr_cdr = cdr->cdr().cast<LispCons>();
            auto cdr_cdr_car = cdr_cdr->car(); // 1
            CUT_ASSERT.isTrue(cdr_cdr_car->is<LispInteger>(), "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(cdr_cdr_car.cast<LispInteger>()->value() == 1, "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(isNil(cdr_cdr->cdr()), "Cdr or Cdr of Cdr is supposed to be nil!");
        }

        // same as above, with more whitespace!
        {
            String input("  \n ( \t \n  \ttheSymbol \t\t    \n\r\n\n\n \r   x \t  \t\t1   )   \r\r\r\r   \t\t ");
            auto consObject = reader.parseList(input.GetIteratorFront());
            auto cons = consObject.cast<LispCons>();
            CUT_ASSERT.isTrue(!isNil(consObject), "Invalid result for parsing the list. (expected cons, got nil)");
            CUT_ASSERT.isTrue(consObject->toString().IsEqual("(theSymbol x 1)"), "Wrong string representation for read list!");
            auto car = cons->car(); // "theSymbol"
            CUT_ASSERT.isTrue(car->is<LispSymbol>(), "First car is supposed to be the symbol 'theSymbol'.");
            CUT_ASSERT.isTrue(car.cast<LispSymbol>()->value().IsEqual("theSymbol"), "First car is supposed to be the symbol 'theSymbol'.");
            CUT_ASSERT.isTrue(cons->cdr()->is<LispCons>(), "The Cdr is supposed to be a cons!.");
            auto cdr = cons->cdr().cast<LispCons>();
            auto cdr_car = cdr->car(); // "x"
            CUT_ASSERT.isTrue(cdr_car->is<LispSymbol>(), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr_car.cast<LispSymbol>()->value().IsEqual("x"), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr->cdr()->is<LispCons>(), "First car is supposed to be the symbol 'theSymbol'.");
            auto cdr_cdr = cdr->cdr().cast<LispCons>();
            auto cdr_cdr_car = cdr_cdr->car(); // 1
            CUT_ASSERT.isTrue(cdr_cdr_car->is<LispInteger>(), "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(cdr_cdr_car.cast<LispInteger>()->value() == 1, "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(isNil(cdr_cdr->cdr()), "Cdr or Cdr of Cdr is supposed to be nil!");
        }
        {
            String input("(theSymbol (x a b) (+ a b))");
            auto consObject = reader.parseList(input.GetIteratorFront());
            CUT_ASSERT.isTrue(consObject->toString().IsEqual("(theSymbol (x a b) (+ a b))"), "Wrong string representation for read list!");
        }
        {
            String input("(lambda(x a b) (+ a b))");
            auto consObject = reader.parseList(input.GetIteratorFront());
            CUT_ASSERT.isTrue(consObject->toString().IsEqual("(lambda (x a b) (+ a b))"), "Wrong string representation for read list!");
        }
    });

    UnitTest g_test9(g_group, "checkSyntax", []{
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String input("()");
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
            CUT_ASSERT.isTrue(result.isComplete());
        }
        {
            String input("())");
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isFalse(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == -1);
            CUT_ASSERT.isFalse(result.isComplete());
        }
        {
            String input("(()");
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 1);
            CUT_ASSERT.isFalse(result.isComplete());
        }
        {
            String input("()()");
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
            CUT_ASSERT.isTrue(result.isComplete());
        }
        {
            String input("(define (a b c) (* (+ b 1) ( - 1 c)))");
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
            CUT_ASSERT.isTrue(result.isComplete());
        }
        {
            // ----------------------------v one closing bracket too much
            String input("(define (a b c)) (* (+ b 1) ( - 1 c)) '123)456");
            // --------------------------------------------------^ cursor should point to this ')' character
            auto result = reader.checkBasicSyntax(input.GetIteratorFront());
            CUT_ASSERT.isFalse(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == -1);
            CUT_ASSERT.isFalse(result.isComplete());
            auto iter = input.GetIteratorFront();
            iter += result.cursor.currentPosition().streamIndex - 1;
            CUT_ASSERT.isTrue(iter.GetCharacter() == '3');
            ++iter;
            CUT_ASSERT.isTrue(iter.GetCharacter() == ')');
            ++iter;
            CUT_ASSERT.isTrue(iter.GetCharacter() == '4');
        }
    });

    UnitTest g_test10(g_group, "skipComments", []{
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String input(";This is a comment\n1337");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispInteger>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispInteger>()->value() == 1337, "Wrong value");
        }
    });

    UnitTest g_test11(g_group, "Detect +- as symbols", []{
        auto pRuntime = resetRuntime();
        auto& reader = *pRuntime->reader();

        {
            String input("(+ 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected + to be read as symbol!");
        }

        {
            String input("(++ 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected ++ to be read as symbol!");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car().cast<LispSymbol>()->value().IsEqual("++"), "Expected ++ to be the symbol value!");
        }

        {
            String input("(+---++++++--++++----++++ 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected +---++++++--++++----++++ to be read as symbol!");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car().cast<LispSymbol>()->value().IsEqual("+---++++++--++++----++++"), "Expected +---++++++--++++----++++ to be the symbol value!");
        }

        {
            String input("(+-++---+++asbndjkh 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected +-++---+++asbndjkh to be read as symbol!");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car().cast<LispSymbol>()->value().IsEqual("+-++---+++asbndjkh"), "Expected +-++---+++asbndjkh to be the symbol value!");
        }

        {
            String input("(- 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected - to be read as symbol!");
        }

        {
            String input("(-- 1 2)");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispCons>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car()->is<LispSymbol>(), "Expected -- to be read as symbol!");
            CUT_ASSERT.isTrue(result.cast<LispCons>()->car().cast<LispSymbol>()->value().IsEqual("--"), "Expected -- to be the symbol value!");
        }

        {
            String input("+1");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispInteger>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispInteger>()->value() == 1, "Expected +1 to be read as integer!");
        }

        {
            String input("-1");
            auto iter = input.GetIteratorFront();
            auto result = reader.read(iter);
            CUT_ASSERT.isTrue(result->is<LispInteger>(), "Wrong type");
            CUT_ASSERT.isTrue(result.cast<LispInteger>()->value() == -1, "Expected -1 to be read as integer!");
        }
    });
}
