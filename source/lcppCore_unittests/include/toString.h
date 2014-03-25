#pragma once

namespace lcpp { namespace unittests {

    template<typename T>
    struct ToStringHelper
    {
        static std::wstring get(const T& object)
        {
            static_cast<const SchemeObject&>(object);

            ezStringWChar result(object.toString().GetData());
            return result.GetData();
        }
    };

    template<typename T>
    struct ToStringHelper<SchemeNumber_t<T>>
    {
        static std::wstring get(const SchemeNumber_t<T>& number)
        {
            ezStringWChar result = number.toString().GetData();
            return result.GetData();
        }
    };

}}

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {
    
    template<>
    static std::wstring ToString<lcpp::Type>(const lcpp::Type& type)
    {
        ezStringWChar wide(type.name);
        return wide.GetData();
    }

    template<>
    static std::wstring ToString<lcpp::SchemeObject>(const lcpp::SchemeObject& object)
    {
        return lcpp::unittests::ToStringHelper<lcpp::SchemeObject>::get(object);
    }

    template<>
    static std::wstring ToString<lcpp::SchemeVoid>(const lcpp::SchemeVoid& theVoid)
    {
        return lcpp::unittests::ToStringHelper<lcpp::SchemeVoid>::get(theVoid);
    }

    template<>
    static std::wstring ToString<lcpp::SchemeBool>(const lcpp::SchemeBool& theBool)
    {
        return lcpp::unittests::ToStringHelper<lcpp::SchemeBool>::get(theBool);
    }

    template<>
    static std::wstring ToString<lcpp::SchemeNil>(const lcpp::SchemeNil& nil)
    {
        return lcpp::unittests::ToStringHelper<lcpp::SchemeNil>::get(nil);
    }

    template<typename T>
    static std::wstring ToString(const lcpp::SchemeNumber_t<T>& number)
    {
        return lcpp::unittests::ToStringHelper< lcpp::SchemeNumber_t<T> >::get(number);
    }

// TODO: More here

}}}
