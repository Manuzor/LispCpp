#pragma once

namespace lcpp
{
    template<typename T>
    struct Ptr
    {
        Ptr(); // [tested]
        Ptr(T* pPtr); // [tested]
        Ptr(const Ptr& rhs); // [tested]
        Ptr(Ptr&& rhs); // [tested]

        ~Ptr();

        void operator =(const Ptr& toCopy); // [tested]
        void operator =(Ptr&& toMove); // [tested]
        void operator =(T* pPtr); // [tested]

        T* operator ->() const; // [tested]
        T& operator *() const; // [tested]

        T* get() const; // [tested]
        bool valid() const; // [tested]

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
}


#include "lcpp/foundation/implementation/ptr.inl"
