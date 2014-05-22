#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("ReaderTests");

    UnitTest g_test1(g_group, "IsSeparator", []()
    {
        Reader reader;

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
        Reader reader;
        ezString str = "   abc";
        auto iter = str.GetIteratorFront();

        reader.skipSeparators(iter);
        CUT_ASSERT.isTrue(iter.GetCharacter() == 'a', "Unable to skip separators.");
    });

    UnitTest g_test3(g_group, "Read", []()
    {
        Reader reader;

        {
            auto& object = reader.read("42");

            auto& staticType = TypeInfo<SchemeInteger>::type();
            auto& type = object.type();

            CUT_ASSERT.isTrue(type == staticType, "Type ID mismatch for scheme integer!");
            CUT_ASSERT.isTrue(dynamic_cast<SchemeInteger*>(&object) != nullptr, "Reader did not return a scheme integer instance!");
            CUT_ASSERT.isTrue(object.toString().IsEqual("42"), "Wrong string representation");
        }

        {
            SchemeObject& object = reader.read("3.1415");

            CUT_ASSERT.isTrue(dynamic_cast<SchemeNumber*>(&object) != nullptr, "Reader did not return a scheme number instance!");
            CUT_ASSERT.isTrue(object.type() == TypeInfo<SchemeNumber>::type(), "Type ID mismatch for scheme number!");
            CUT_ASSERT.isTrue(object.toString().IsEqual("3.1415"), "Wrong string representation");
        }

        {
            SchemeObject& object = reader.read("hello world");

            CUT_ASSERT.isTrue(dynamic_cast<SchemeSymbol*>(&object) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(object.type() == TypeInfo<SchemeSymbol>::type(), "Type ID mismatch for scheme symbol!");
            CUT_ASSERT.isTrue(object.toString().IsEqual("hello"), "Wrong string representation");
        }

        {
            SchemeObject& object = reader.read("\"hello world\"");

            CUT_ASSERT.isTrue(dynamic_cast<SchemeString*>(&object) != nullptr, "Reader did not return a scheme string instance!");
            CUT_ASSERT.isTrue(object.type() == TypeInfo<SchemeString>::type(), "Type ID mismatch for scheme string!");
            CUT_ASSERT.isTrue(object.toString().IsEqual("\"hello world\""), "Wrong string representation");
        }
    });

    UnitTest g_test4(g_group, "parseInteger", []()
    {
        Reader reader;

        {
            ezString str("123");
            auto& i = reader.parseInteger(str.GetIteratorFront());
            CUT_ASSERT.isTrue(i.value() == 123, "Invalid value of parsed integer.");
            CUT_ASSERT.isTrue(i.toString().IsEqual("123"), "Invalid string representation of parsed integer.");
        }

        {
            ezString str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseInteger(str.GetIteratorFront());
            }, "'qwerty' should not be parsed as integer!");
        }
    });

    UnitTest g_test5(g_group, "parseNumber", []()
    {
        Reader reader;

        {
            ezString str("3.1415");
            auto number = reader.parseNumber(str.GetIteratorFront());
            CUT_ASSERT.isTrue(number.value() == 3.1415, "Invalid value of parsed number.");
            CUT_ASSERT.isTrue(number.toString().IsEqual("3.1415"), "Invalid string representation of parsed number.");
        }

        {
            ezString str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseNumber(str.GetIteratorFront());
            }, "'qwerty' should not be parsed as number!");
        }
    });

    UnitTest g_test6(g_group, "parseSymbol", []()
    {
        Reader reader;

        {
            ezString str("qwerty");
            auto& symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol.value().IsEqual("qwerty"), "Invalid value of parsed symbol.");
        }

        {
            ezString str("hello world with spaces");
            auto& symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol.value().IsEqual("hello"), "Invalid value of parsed symbol. It should not consume spaces");
        }

        {
            ezString str("    leadingWhiteSpace");
            auto& symbol = reader.parseSymbol(str.GetIteratorFront());
            CUT_ASSERT.isTrue(symbol.value().IsEqual("leadingWhiteSpace"), "Invalid value of parsed symbol.");
        }

        {
            ezString str("123");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseSymbol(str.GetIteratorFront());
            }, "'123' should not be parsed as symbol!");
        }
    });

    UnitTest g_test7(g_group, "parseString", []()
    {
        Reader reader;

        {
            ezString input("\"qwerty\"");
            auto& str = reader.parseString(input.GetIteratorFront());
            CUT_ASSERT.isTrue(str.value().IsEqual("qwerty"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str.toString().IsEqual("\"qwerty\""), "Invalid value of parsed string.");
        }

        {
            ezString input("\"123\"");
            auto& str = reader.parseString(input.GetIteratorFront());
            CUT_ASSERT.isTrue(str.value().IsEqual("123"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str.toString().IsEqual("\"123\""), "Invalid value of parsed string.");
        }

        {
            ezString input("hello");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseString(input.GetIteratorFront());
            }, "'hello' should not be parsed as string! ('\"hello\"' should)");
        }
    });

    UnitTest g_test8(g_group, "parseList", []()
    {
        Reader reader;

        {
            ezString input("hello world 123 42");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseList(input.GetIteratorFront());
            }, "The input string 'hello world 123 42' should not be read as a valid list!");
        }

        {
            ezString input("()");
            auto& consObject = reader.parseList(input.GetIteratorFront());
            CUT_ASSERT.isTrue(consObject == SCHEME_NIL, "Invalid result for parsing the list.");
        }

        {
            ezString input("(define x 1)");
            auto& consObject = reader.parseList(input.GetIteratorFront());
            auto& cons = static_cast<SchemeCons&>(consObject);
            CUT_ASSERT.isTrue(!isNil(consObject), "Invalid result for parsing the list. (expected cons, got nil)");
            CUT_ASSERT.isTrue(consObject.toString().IsEqual("(define x 1)"), "Wrong string representation for read list!");
            auto& car = cons.car(); // "define"
            CUT_ASSERT.isTrue(car.is<SchemeSymbol>(), "First car is supposed to be the symbol 'define'.");
            CUT_ASSERT.isTrue(static_cast<SchemeSymbol&>(car).value().IsEqual("define"), "First car is supposed to be the symbol 'define'.");
            CUT_ASSERT.isTrue(cons.cdr().is<SchemeCons>(), "The Cdr is supposed to be a cons!.");
            auto& cdr = static_cast<SchemeCons&>(cons.cdr());
            auto& cdr_car = cdr.car(); // "x"
            CUT_ASSERT.isTrue(cdr_car.is<SchemeSymbol>(), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(static_cast<SchemeSymbol&>(cdr_car).value().IsEqual("x"), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr.cdr().is<SchemeCons>(), "First car is supposed to be the symbol 'define'.");
            auto& cdr_cdr = static_cast<SchemeCons&>(cdr.cdr());
            auto& cdr_cdr_car = cdr_cdr.car(); // 1
            CUT_ASSERT.isTrue(cdr_cdr_car.is<SchemeInteger>(), "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(static_cast<SchemeInteger&>(cdr_cdr_car).value() == 1, "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(isNil(cdr_cdr.cdr()), "Cdr or Cdr of Cdr is supposed to be nil!");
        }

        // same as above, with more whitespace!
        {
            ezString input("  \n ( \t \n  \tdefine \t\t    \n\r\n\n\n \r   x \t  \t\t1   )   \r\r\r\r   \t\t ");
            auto& consObject = reader.parseList(input.GetIteratorFront());
            auto& cons = static_cast<SchemeCons&>(consObject);
            CUT_ASSERT.isTrue(!isNil(consObject), "Invalid result for parsing the list. (expected cons, got nil)");
            CUT_ASSERT.isTrue(consObject.toString().IsEqual("(define x 1)"), "Wrong string representation for read list!");
            auto& car = cons.car(); // "define"
            CUT_ASSERT.isTrue(car.is<SchemeSymbol>(), "First car is supposed to be the symbol 'define'.");
            CUT_ASSERT.isTrue(static_cast<SchemeSymbol&>(car).value().IsEqual("define"), "First car is supposed to be the symbol 'define'.");
            CUT_ASSERT.isTrue(cons.cdr().is<SchemeCons>(), "The Cdr is supposed to be a cons!.");
            auto& cdr = static_cast<SchemeCons&>(cons.cdr());
            auto& cdr_car = cdr.car(); // "x"
            CUT_ASSERT.isTrue(cdr_car.is<SchemeSymbol>(), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(static_cast<SchemeSymbol&>(cdr_car).value().IsEqual("x"), "Second car is supposed to be the symbol 'x'.");
            CUT_ASSERT.isTrue(cdr.cdr().is<SchemeCons>(), "First car is supposed to be the symbol 'define'.");
            auto& cdr_cdr = static_cast<SchemeCons&>(cdr.cdr());
            auto& cdr_cdr_car = cdr_cdr.car(); // 1
            CUT_ASSERT.isTrue(cdr_cdr_car.is<SchemeInteger>(), "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(static_cast<SchemeInteger&>(cdr_cdr_car).value() == 1, "Third car is supposed to be the integer 1.");
            CUT_ASSERT.isTrue(isNil(cdr_cdr.cdr()), "Cdr or Cdr of Cdr is supposed to be nil!");
        }
        {
            ezString input("(define (x a b) (+ a b))");
            auto& consObject = reader.parseList(input.GetIteratorFront());
            CUT_ASSERT.isTrue(consObject.toString().IsEqual("(define (x a b) (+ a b))"), "Wrong string representation for read list!");
        }
    });

    UnitTest g_test9(g_group, "checkSyntax", [](){
        Reader reader;

        {
            ezString input("()");
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
        }
        {
            ezString input("())");
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isFalse(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == -1);
        }
        {
            ezString input("(()");
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 1);
        }
        {
            ezString input("()()");
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
        }
        {
            ezString input("(define (a b c) (* (+ b 1) ( - 1 c)))");
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isTrue(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == 0);
        }
        {
            // ----------------------------v one closing bracket too much
            ezString input("(define (a b c)) (* (+ b 1) ( - 1 c)) '123)456");
            // --------------------------------------------------^ cursor should point to this ')' character
            auto result = reader.checkSyntax(input.GetIteratorFront());
            CUT_ASSERT.isFalse(result.valid);
            CUT_ASSERT.isTrue(result.parenthesisBalance == -1);
            auto iter = input.GetIteratorFront();
            iter += result.cursor.streamIndex - 1;
            CUT_ASSERT.isTrue(iter.GetCharacter() == '3');
            ++iter;
            CUT_ASSERT.isTrue(iter.GetCharacter() == ')');
            ++iter;
            CUT_ASSERT.isTrue(iter.GetCharacter() == '4');
        }

    });
}
