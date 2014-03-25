#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    template<typename NUMBER_TYPE>
    class SchemeNumber_t :
        public SchemeExtend<SchemeNumber_t<NUMBER_TYPE>, SchemeObject>
    {
    public:
        typedef NUMBER_TYPE number_t;
        typedef SchemeNumber_t<number_t> type_t;

        SchemeNumber_t(number_t value);
        virtual ~SchemeNumber_t();

        virtual bool operator ==(const SchemeObject& obj) const override;

        bool operator ==(const type_t& other) const;

        number_t value() const;
        void value(number_t value);

        virtual ezString toString() const override;

        // Other operators
        // Note: I cannot move these to the _inl file due to a compiler bug...

        // conversion
        //////////////////////////////////////////////////////////////////////////
        template<typename T>
        inline operator T()
        {
            return (T)m_value;
        }

        // =
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline type_t& operator =(T rhs)
        {
            m_value = (number_t)rhs;
            return *this;
        }

        // + += ++
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline T operator +(T rhs) const
        {
            return m_value + rhs;
        }

        template<typename T>
        inline T operator +(SchemeNumber_t<T> rhs) const
        {
            return m_value + (T)rhs;
        }

        template<typename T>
        inline type_t operator +=(T rhs)
        {
            m_value = m_value + rhs;
            return *this;
        }

        template<typename T>
        inline type_t operator +=(SchemeNumber_t<T> rhs)
        {
            m_value = m_value + (T)rhs;
            return *this;
        }

        // prefix
        inline type_t& operator ++()
        {
            m_value++;
            return *this;
        }

        // postfix
        template<typename T>
        inline type_t operator ++(T)
        {
            type_t ret(*this);
            operator++();
            return ret;
        }

        // - -= --
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline T operator -(T rhs) const
        {
            return m_value - rhs;
        }

        template<typename T>
        inline T operator -(SchemeNumber_t<T> rhs) const
        {
            return m_value - (T)rhs;
        }

        template<typename T>
        inline type_t operator -=(T rhs)
        {
            m_value = m_value - rhs;
            return *this;
        }

        template<typename T>
        inline type_t operator -=(SchemeNumber_t<T> rhs)
        {
            m_value = m_value - (T)rhs;
            return *this;
        }

        // prefix
        inline type_t& operator --()
        {
            m_value--;
            return *this;
        }

        // postfix
        template<typename T>
        inline type_t operator --(T)
        {
            type_t ret(*this);
            operator--();
            return ret;
        }

        // * *=
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline T operator *(T rhs) const
        {
            return m_value * rhs;
        }

        template<typename T>
        inline T operator *(SchemeNumber_t<T> rhs) const
        {
            return m_value * (T)rhs;
        }

        template<typename T>
        inline type_t operator *=(T rhs)
        {
            m_value = m_value * rhs;
            return *this;
        }

        template<typename T>
        inline type_t operator *=(SchemeNumber_t<T> rhs)
        {
            m_value = m_value * (T)rhs;
            return *this;
        }

        // / /=
        //////////////////////////////////////////////////////////////////////////

        template<typename T>
        inline T operator /(T rhs) const
        {
            return m_value / rhs;
        }

        template<typename T>
        inline T operator /(SchemeNumber_t<T> rhs) const
        {
            return m_value / (T)rhs;
        }

        template<typename T>
        inline type_t operator /=(T rhs)
        {
            m_value = m_value / rhs;
            return *this;
        }

        template<typename T>
        inline type_t operator /=(SchemeNumber_t<T> rhs)
        {
            m_value = m_value / (T)rhs;
            return *this;
        }

    private:
        number_t m_value;
    };

    typedef SchemeNumber_t<ezInt8> SchemeInt8;
    typedef SchemeNumber_t<ezUInt8> SchemeUInt8;

    typedef SchemeNumber_t<ezInt16> SchemeInt16;
    typedef SchemeNumber_t<ezUInt16> SchemeUInt16;

    typedef SchemeNumber_t<ezInt32> SchemeInt32;
    typedef SchemeNumber_t<ezUInt32> SchemeUInt32;

    typedef SchemeNumber_t<ezInt64> SchemeInt64;
    typedef SchemeNumber_t<ezUInt64> SchemeUInt64;

    typedef SchemeNumber_t<float> SchemeFloat;
    typedef SchemeNumber_t<double> SchemeDouble;

    template<typename T>
    struct TypeInfo< SchemeNumber_t<T> >
    {
        static const Type& type()
        {
            static Type instance;
            instance.name = "SchemeNumber";
            instance.size = sizeof(SchemeNumber_t<T>);
            instance.alignment = EZ_ALIGNMENT_OF(SchemeNumber_t<T>);
            return instance;
        }                                                 
    };
}

#include "lcpp/core/typeSystem/types/implementation/number_inl.h"
