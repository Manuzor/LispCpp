#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    template<typename NUMBER_TYPE>
    class LispNumber_t :
        public LispExtend<LispNumber_t<NUMBER_TYPE>, LispObject>
    {
        friend class TypeFactory;
    public:
        typedef NUMBER_TYPE Number_t;

        static_assert(std::is_arithmetic<Number_t>::value,
            "Tried to create LispNumber_t of a type other than the built in ones!");

        virtual ~LispNumber_t();

        virtual bool operator ==(const LispObject& obj) const LCPP_OVERRIDE;

        Number_t value() const;
        void     value(Number_t val);

        virtual ezString toString() const LCPP_OVERRIDE;

        // Other operators
        // Note: Due to a compiler bug, they can't be put in the _inl file.
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline LispNumber_t<Number_t>& operator +=(T rhs)
        {
            m_value = Number_t(m_value + rhs);
            return *this;
        }

        template<typename T>
        inline LispNumber_t<Number_t>& operator -=(T rhs)
        {
            m_value = Number_t(m_value - rhs);
            return *this;
        }

        template<typename T>
        inline LispNumber_t<Number_t>& operator *=(T rhs)
        {
            m_value = Number_t(m_value * rhs);
            return *this;
        }

        template<typename T>
        inline LispNumber_t<Number_t>& operator /=(T rhs)
        {
            m_value = Number_t(m_value / rhs);
            return *this;
        }

        // prefix
        //////////////////////////////////////////////////////////////////////////
        inline LispNumber_t<Number_t>& operator ++() { ++m_value; return *this; }
        inline LispNumber_t<Number_t>& operator --() { --m_value; return *this; }

        // postfix
        //////////////////////////////////////////////////////////////////////////
        inline LispNumber_t<Number_t> operator ++(int)
        {
            LispNumber_t<Number_t> old(*this);
            operator++();
            return old;
        }
        inline LispNumber_t<Number_t> operator --(int)
        {
            LispNumber_t<Number_t> old(*this);
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
        inline LispNumber_t<Number_t>& operator =(const LispNumber_t<T>& rhs)
        {
            m_value = Number_t(rhs.value());
            return *this;
        }

    private:
        Number_t m_value;

        /// Deliberately not explicit
        explicit LispNumber_t(Number_t value);
        void operator = (const LispNumber_t<Number_t>&);
    };

    // LispNumber_t<> arithmetic operators
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    bool operator ==(const LispNumber_t<T>& lhs, const LispNumber_t<U>& rhs)
    {
        return lhs.value() == rhs.value();
    }
    
    template<typename T, typename U>
    bool operator ==(LispNumber_t<T> lhs, U rhs)
    {
        return lhs.value() == rhs;
    }

    
#define LCPP_DEFINE_SCHEME_NUMBER_ARITHMETIC_OPERATOR(op)                 \
    template<typename T, typename U>                                      \
    inline auto operator op(LispNumber_t<T> lhs, LispNumber_t<U> rhs) \
        -> decltype(lhs.value() op rhs.value())                           \
    {                                                                     \
        return lhs.value() op rhs.value();                                \
    }                                                                     \
    template<typename T, typename U>                                      \
    inline auto operator op(LispNumber_t<T> lhs, U rhs)                 \
        -> decltype(lhs.value() op rhs)                                   \
    {                                                                     \
        return lhs.value() op rhs;                                        \
    }                                                                     \
    template<typename T, typename U>                                      \
    inline auto operator op(T lhs, LispNumber_t<U> rhs)                 \
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

    typedef LispNumber_t<ezInt64> LispInteger;
    typedef LispNumber_t<double> LispNumber;

    // Type info definition
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct TypeInfo<LispInteger>
    {
        static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static Type integerType = Type::create(
                "LispInteger",
                lcpp::MemoryInfo(sizeof(lcpp::LispInteger), EZ_ALIGNMENT_OF(lcpp::LispInteger))
                );
            return integerType;
        }
    };

    template<>
    struct LCPP_CORE_API TypeInfo<LispNumber>
    {
        static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static Type numberInstance = Type::create(
                "LispNumber",
                MemoryInfo(sizeof(LispNumber), EZ_ALIGNMENT_OF(LispNumber))
                );
            return numberInstance;
        }
    };
}

#include "lcpp/core/typeSystem/types/implementation/number_inl.h"
