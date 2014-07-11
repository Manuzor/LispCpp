#pragma once

namespace lcpp
{
    template<typename T>
    struct Ptr
    {
        // Default construct as nullptr
        Ptr(); // [tested]

        // Copy constructor
        Ptr(const Ptr& rhs); // [tested]

        // Move constructor
        Ptr(Ptr&& rhs); // [tested]

        // Construct from raw pointer
        Ptr(T* pPtr); // [tested]

        // Construct from other Ptr<>
        template<typename T_Other>
        Ptr(const Ptr<T_Other>& pOther);

        ~Ptr();

        void operator =(const Ptr& toCopy); // [tested]
        void operator =(Ptr&& toMove); // [tested]
        void operator =(T* pPtr); // [tested]

        T* operator ->() const; // [tested]
        T& operator *() const; // [tested]

        T* get() const; // [tested]
        bool isNull() const; // [tested]

        template<typename T_Other>
        Ptr<T_Other> cast() const; // [tested]

        operator bool() const; // [tested]

    private:
        T* m_pPtr;
    };

    template<typename T>
    bool operator !(const Ptr<T>& ptr); // [tested]

    template<typename T_Rhs, typename T_Lhs>
    Ptr<T_Rhs> cast(const Ptr<T_Lhs>& lhs); // [tested]

    template<typename T_Lhs, typename T_Rhs>
    bool operator == (Ptr<T_Lhs> lhs, Ptr<T_Rhs> rhs);

    template<typename T_Lhs, typename T_Rhs>
    bool operator != (Ptr<T_Lhs> lhs, Ptr<T_Rhs> rhs);
}

#include "lcpp/foundation/implementation/ptr.inl"
