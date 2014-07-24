#pragma once

namespace lcpp { namespace unittests {

    template<typename T>
    struct ToStringHelper
    {
        static std::wstring get(const T& object)
        {
            static_cast<const LispObject&>(object);

            ezStringWChar result(object.toString().GetData());
            return result.GetData();
        }
    };

    template<typename T>
    struct ToStringHelper<LispNumber_t<T>>
    {
        static std::wstring get(const LispNumber_t<T>& number)
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
    static std::wstring ToString<lcpp::LispObject>(const lcpp::LispObject& object)
    {
        return lcpp::unittests::ToStringHelper<lcpp::LispObject>::get(object);
    }

    template<>
    static std::wstring ToString<lcpp::LispVoid>(const lcpp::LispVoid& theVoid)
    {
        return lcpp::unittests::ToStringHelper<lcpp::LispVoid>::get(theVoid);
    }

    template<>
    static std::wstring ToString<lcpp::LispBool>(const lcpp::LispBool& theBool)
    {
        return lcpp::unittests::ToStringHelper<lcpp::LispBool>::get(theBool);
    }

    template<>
    static std::wstring ToString<lcpp::LispNil>(const lcpp::LispNil& nil)
    {
        return lcpp::unittests::ToStringHelper<lcpp::LispNil>::get(nil);
    }

    template<typename T>
    static std::wstring ToString(const lcpp::LispNumber_t<T>& number)
    {
        return lcpp::unittests::ToStringHelper< lcpp::LispNumber_t<T> >::get(number);
    }

    template<>
    static std::wstring ToString<lcpp::LispSymbol>(const lcpp::LispSymbol& symbol)
    {
        return lcpp::unittests::ToStringHelper<lcpp::LispSymbol>::get(symbol);
    }

}}}
