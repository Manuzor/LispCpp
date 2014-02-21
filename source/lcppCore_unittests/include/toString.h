#pragma once
#define LCPP_UNITTESTS_FRAMEWORK_TOSTRING(typeName) namespace Microsoft {\
namespace VisualStudio {\
namespace CppUnitTestFramework { \
    template<>                                                    \
    static std::wstring ToString<typeName>(const typeName& value) \
    {                                                             \
        ezStringWChar result = value.toString().GetData();        \
        return result.GetData();                                  \
    }                                                             \
}}}

LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeObject);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeVoid);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeBool);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeNil);

LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeInt8);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeUInt8);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeInt16);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeUInt16);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeInt32);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeUInt32);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeInt64);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeUInt64);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeFloat);
LCPP_UNITTESTS_FRAMEWORK_TOSTRING(lcpp::SchemeDouble);

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {

    template<>
    static std::wstring ToString<lcpp::SchemeType::Enum>(const lcpp::SchemeType::Enum& value)
    {
        static_assert(lcpp::SchemeType::NUM_ELEMENTS == 8, "Keep this method in sync with lcpp::SchemeType::Enum!");
        switch (value)
        {
        case lcpp::SchemeType::Object: return L"Object";
        case lcpp::SchemeType::Void:   return L"Void";
        case lcpp::SchemeType::Nil:    return L"Nil";
        case lcpp::SchemeType::Bool:   return L"Bool";
        case lcpp::SchemeType::Symbol: return L"Symbol";
        case lcpp::SchemeType::Cons:   return L"Cons";
        case lcpp::SchemeType::Number: return L"Number";
        case lcpp::SchemeType::String: return L"String";
        default: return L"<INVALID>";
        }
    }

// TODO: More here

}}}
