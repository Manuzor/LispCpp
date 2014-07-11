#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    template<typename NUMBER_TYPE>
    class SchemeNumber_t :
        public SchemeExtend<SchemeNumber_t<NUMBER_TYPE>, SchemeObject>
    {
        friend class TypeFactory;
    public:
        typedef NUMBER_TYPE Number_t;

        static_assert(std::is_arithmetic<Number_t>::value,
            "Tried to create SchemeNumber_t of a type other than the built in ones!");

        virtual ~SchemeNumber_t();

        virtual bool operator ==(const SchemeObject& obj) const LCPP_OVERRIDE;

        Number_t value() const;
        void     value(Number_t val);

        virtual ezString toString() const LCPP_OVERRIDE;

        // Other operators
        // Note: Due to a compiler bug, they can't be put in the _inl file.
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline SchemeNumber_t<Number_t>& operator +=(T rhs)
        {
            m_value = Number_t(m_value + rhs);
            return *this;
        }

        template<typename T>
        inline SchemeNumber_t<Number_t>& operator -=(T rhs)
        {
            m_value = Number_t(m_value - rhs);
            return *this;
        }

        template<typename T>
        inline SchemeNumber_t<Number_t>& operator *=(T rhs)
        {
            m_value = Number_t(m_value * rhs);
            return *this;
        }

        template<typename T>
        inline SchemeNumber_t<Number_t>& operator /=(T rhs)
        {
            m_value = Number_t(m_value / rhs);
            return *this;
        }

        // prefix
        //////////////////////////////////////////////////////////////////////////
        inline SchemeNumber_t<Number_t>& operator ++() { ++m_value; return *this; }
        inline SchemeNumber_t<Number_t>& operator --() { --m_value; return *this; }

        // postfix
        //////////////////////////////////////////////////////////////////////////
        inline SchemeNumber_t<Number_t> operator ++(int)
        {
            SchemeNumber_t<Number_t> old(*this);
            operator++();
            return old;
        }
        inline SchemeNumber_t<Number_t> operator --(int)
        {
            SchemeNumber_t<Number_t> old(*this);
            operator--();
            return old;
        }

        // conversion
        //////////////////////////////////////////////////////////////////////////
        //template<typename T>
        //inline operator T() // TODO const?
        //{
        //    return static_cast<T>(m_value);
        //}

        // =
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline SchemeNumber_t<Number_t>& operator =(const SchemeNumber_t<T>& rhs)
        {
            m_value = Number_t(rhs.value());
            return *this;
        }

    private:
        Number_t m_value;

        /// Deliberately not explicit
        explicit SchemeNumber_t(Number_t value);
        void operator = (const SchemeNumber_t<Number_t>&);
    };

    // SchemeNumber_t<> arithmetic operators
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    bool operator ==(const SchemeNumber_t<T>& lhs, const SchemeNumber_t<U>& rhs)
    {
        return lhs.value() == rhs.value();
    }
    
    template<typename T, typename U>
    bool operator ==(SchemeNumber_t<T> lhs, U rhs)
    {
        return lhs.value() == rhs;
    }

    
#define LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(op)                 \
    template<typename T, typename U>                                      \
    inline auto operator op(SchemeNumber_t<T> lhs, SchemeNumber_t<U> rhs) \
        -> decltype(lhs.value() op rhs.value())                           \
    {                                                                     \
        return lhs.value() op rhs.value();                                \
    }                                                                     \
    template<typename T, typename U>                                      \
    inline auto operator op(SchemeNumber_t<T> lhs, U rhs)                 \
        -> decltype(lhs.value() op rhs)                                   \
    {                                                                     \
        return lhs.value() op rhs;                                        \
    }                                                                     \
    template<typename T, typename U>                                      \
    inline auto operator op(T lhs, SchemeNumber_t<U> rhs)                 \
        -> decltype(lhs op rhs.value())                                   \
    {                                                                     \
        return lhs op rhs.value();                                        \
    }

    LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(+);
    LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(-);
    LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(*);
    LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(/);

#undef LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR
    
    // Convenience typedefs
    //////////////////////////////////////////////////////////////////////////

    typedef SchemeNumber_t<ezInt64> SchemeInteger;
    typedef SchemeNumber_t<double> SchemeNumber;

    // Type info definition
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct TypeInfo<SchemeInteger>
    {
        static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static Type integerType = Type::create(
                "SchemeInteger",
                lcpp::MemoryInfo(sizeof(lcpp::SchemeInteger), EZ_ALIGNMENT_OF(lcpp::SchemeInteger))
                );
            return integerType;
        }
    };

    template<>
    struct LCPP_CORE_API TypeInfo<SchemeNumber>
    {
        static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static Type numberInstance = Type::create(
                "SchemeNumber",
                MemoryInfo(sizeof(SchemeNumber), EZ_ALIGNMENT_OF(SchemeNumber))
                );
            return numberInstance;
        }
    };
}

#include "lcpp/core/typeSystem/types/implementation/number_inl.h"
