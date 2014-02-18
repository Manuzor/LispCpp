#pragma once

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {

    template<>
    static std::wstring ToString<lcpp::SchemeObject>(const lcpp::SchemeObject& value)
    {
        ezStringWChar result = value.toString().GetData();
        return result.GetData();
    }

    template<>
    static std::wstring ToString<lcpp::SchemeNil>(const lcpp::SchemeNil& value)
    {
        ezStringWChar result = value.toString().GetData();
        return result.GetData();
    }

    template<>
    static std::wstring ToString<lcpp::SchemeBool>(const lcpp::SchemeBool& value)
    {
        ezStringWChar result = value.toString().GetData();
        return result.GetData();
    }

    template<>
    static std::wstring ToString<lcpp::SchemeType::Enum>(const lcpp::SchemeType::Enum& value)
    {
        static_assert(lcpp::SchemeType::NUM_ELEMENTS == 7, "Keep this method in sync with lcpp::SchemeType::Enum!");
        switch (value)
        {
        case lcpp::SchemeType::Object: return L"Object";
        case lcpp::SchemeType::Void:   return L"Void";
        case lcpp::SchemeType::Nil:    return L"Nil";
        case lcpp::SchemeType::Bool:   return L"Bool";
        case lcpp::SchemeType::Cons:   return L"Cons";
        case lcpp::SchemeType::Number: return L"Number";
        case lcpp::SchemeType::String: return L"String";
        default: return L"<INVALID>";
        }
    }

}}}
