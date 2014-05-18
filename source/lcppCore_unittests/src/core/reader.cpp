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
            auto& i = reader.parseInteger(str);
            CUT_ASSERT.isTrue(i.value() == 123, "Invalid value of parsed integer.");
            CUT_ASSERT.isTrue(i.toString().IsEqual("123"), "Invalid string representation of parsed integer.");
        }

        {
            ezString str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseInteger(str);
            }, "'qwerty' should not be parsed as integer!");
        }
    });

    UnitTest g_test5(g_group, "parseNumber", []()
    {
        Reader reader;

        {
            ezString str("3.1415");
            auto number = reader.parseNumber(str);
            CUT_ASSERT.isTrue(number.value() == 3.1415, "Invalid value of parsed number.");
            CUT_ASSERT.isTrue(number.toString().IsEqual("3.1415"), "Invalid string representation of parsed number.");
        }

        {
            ezString str("qwerty");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseNumber(str);
            }, "'qwerty' should not be parsed as number!");
        }
    });

    UnitTest g_test6(g_group, "parseSymbol", []()
    {
        Reader reader;

        {
            ezString str("qwerty");
            auto& symbol = reader.parseSymbol(str);
            CUT_ASSERT.isTrue(symbol.value().IsEqual("qwerty"), "Invalid value of parsed symbol.");
        }

        {
            ezString str("hello world with spaces");
            auto& symbol = reader.parseSymbol(str);
            CUT_ASSERT.isTrue(symbol.value().IsEqual("hello"), "Invalid value of parsed symbol. It should not consume spaces");
        }

        {
            ezString str("123");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseSymbol(str);
            }, "'123' should not be parsed as symbol!");
        }
    });

    UnitTest g_test7(g_group, "parseString", []()
    {
        Reader reader;

        {
            ezString input("\"qwerty\"");
            auto& str = reader.parseString(input);
            CUT_ASSERT.isTrue(str.value().IsEqual("qwerty"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str.toString().IsEqual("\"qwerty\""), "Invalid value of parsed string.");
        }

        {
            ezString input("\"123\"");
            auto& str = reader.parseString(input);
            CUT_ASSERT.isTrue(str.value().IsEqual("123"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(str.toString().IsEqual("\"123\""), "Invalid value of parsed string.");
        }

        {
            ezString input("hello");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseString(input);
            }, "'hello' should not be parsed as string! ('\"hello\"' should)");
        }
    });

    UnitTest g_test8(g_group, "parseList", []()
    {
        Reader reader;

        {
            ezString input("hello world 123 42");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseList(input);
            }, "The input string 'hello world 123 42' should not be read as a valid list!");
        }

        {
            ezString input("(define x 1)");
            auto& cons = reader.parseList(input);
            CUT_ASSERT.isTrue(cons != SCHEME_NIL, "Invalid result for parsing the list.");
        }
    });
}
