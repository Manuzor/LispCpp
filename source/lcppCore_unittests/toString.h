#pragma once

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {

    template<>
    static std::wstring ToString<lcpp::SchemeObject>(const lcpp::SchemeObject& value)
    {
        return static_cast<std::wstring>(value.toString());
    }

    template<>
    static std::wstring ToString<lcpp::SchemeNil>(const lcpp::SchemeNil& value)
    {
        return static_cast<std::wstring>(value.toString());
    }

    template<>
    static std::wstring ToString<lcpp::SchemeBool>(const lcpp::SchemeBool& value)
    {
        return static_cast<std::wstring>(value.toString());
    }

}}}
