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

}}}
