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
            SchemeObject* pObject = reader.read("42");

            auto& staticType = TypeInfo<SchemeInteger>::type();
            auto& type = pObject->type();

            CUT_ASSERT.isTrue(pObject != nullptr, "Reader did not return a valid object!");
            CUT_ASSERT.isTrue(dynamic_cast<SchemeInteger*>(pObject) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(type == staticType, "Type ID mismatch for scheme integer!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("42"), "Wrong string representation");
        }

        {
            SchemeObject* pObject = reader.read("3.1415");

            CUT_ASSERT.isTrue(pObject != nullptr, "Reader did not return a valid object!");
            CUT_ASSERT.isTrue(dynamic_cast<SchemeNumber*>(pObject) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(pObject->type() == TypeInfo<SchemeNumber>::type(), "Type ID mismatch for scheme number!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("3.1415"), "Wrong string representation");
        }

        {
            SchemeObject* pObject = reader.read("hello world");

            CUT_ASSERT.isTrue(pObject != nullptr, "Reader did not return a valid object!");
            CUT_ASSERT.isTrue(dynamic_cast<SchemeSymbol*>(pObject) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(pObject->type() == TypeInfo<SchemeSymbol>::type(), "Type ID mismatch for scheme number!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("hello"), "Wrong string representation");
        }

        {
            SchemeObject* pObject = reader.read("\"hello world\"");

            CUT_ASSERT.isTrue(pObject != nullptr, "Reader did not return a valid object!");
            CUT_ASSERT.isTrue(dynamic_cast<SchemeString*>(pObject) != nullptr, "Reader did not return a scheme symbol instance!");
            CUT_ASSERT.isTrue(pObject->type() == TypeInfo<SchemeString>::type(), "Type ID mismatch for scheme number!");
            CUT_ASSERT.isTrue(pObject->toString().IsEqual("\"hello world\""), "Wrong string representation");
        }
    });

    UnitTest g_test4(g_group, "parseInteger", []()
    {
        Reader reader;

        {
            ezString str("123");
            auto pInt = reader.parseInteger(str);
            CUT_ASSERT.isTrue(pInt != nullptr, "Invalid result for parsing the integer.");
            CUT_ASSERT.isTrue(pInt->value() == 123, "Invalid value of parsed integer.");
            CUT_ASSERT.isTrue(pInt->toString().IsEqual("123"), "Invalid string representation of parsed integer.");
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
            auto pNumber = reader.parseNumber(str);
            CUT_ASSERT.isTrue(pNumber != nullptr, "Invalid result for parsing the number.");
            CUT_ASSERT.isTrue(pNumber->value() == 3.1415, "Invalid value of parsed number.");
            CUT_ASSERT.isTrue(pNumber->toString().IsEqual("3.1415"), "Invalid string representation of parsed number.");
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
            auto pSymbol = reader.parseSymbol(str);
            CUT_ASSERT.isTrue(pSymbol != nullptr, "Invalid result for parsing the symbol.");
            CUT_ASSERT.isTrue(pSymbol->value().IsEqual("qwerty"), "Invalid value of parsed symbol.");
        }

        {
            ezString str("hello world with spaces");
            auto pSymbol = reader.parseSymbol(str);
            CUT_ASSERT.isTrue(pSymbol != nullptr, "Invalid result for parsing the symbol.");
            CUT_ASSERT.isTrue(pSymbol->value().IsEqual("hello"), "Invalid value of parsed symbol. It should not consume spaces");
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
            ezString str("\"qwerty\"");
            auto pString = reader.parseString(str);
            CUT_ASSERT.isTrue(pString != nullptr, "Invalid result for parsing the string.");
            CUT_ASSERT.isTrue(pString->value().IsEqual("qwerty"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(pString->toString().IsEqual("\"qwerty\""), "Invalid value of parsed string.");
        }

        {
            ezString str("\"123\"");
            auto pString = reader.parseString(str);
            CUT_ASSERT.isTrue(pString != nullptr, "Invalid result for parsing the string.");
            CUT_ASSERT.isTrue(pString->value().IsEqual("123"), "Invalid value of parsed string.");
            CUT_ASSERT.isTrue(pString->toString().IsEqual("\"123\""), "Invalid value of parsed string.");
        }

        {
            ezString str("hello");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseString(str);
            }, "'hello' should not be parsed as string! ('\"hello\"' should)");
        }
    });

    UnitTest g_test8(g_group, "ParseList", []()
    {
        Reader reader;

        {
            ezString str("hello world 123 42");
            CUT_ASSERT.throws<lcpp::exceptions::InvalidInput>([&](){
                reader.parseList(str);
            }, "The input string 'hello world 123 42' should not be read as a valid list!");
        }

        {
            ezString str("(define x 1)");
            auto pCons = reader.parseList(str);
            CUT_ASSERT.isTrue(pCons != nullptr, "Invalid result for parsing the list.");
        }
    });
}
