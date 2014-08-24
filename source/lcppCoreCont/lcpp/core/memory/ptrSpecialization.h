#pragma once
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    template<>
    struct Ptr<LispObject>
    {
        // Default construct as nullptr
        Ptr();

        // Copy constructor
        Ptr(const Ptr& rhs);

        // Move constructor
        Ptr(Ptr&& rhs);

        // Construct from raw pointer
        Ptr(LispObject* pPtr);

        ~Ptr();

        void operator =(const Ptr& toCopy);
        void operator =(Ptr&& toMove);
        void operator =(LispObject* pPtr);

        LispObject* operator ->() const;
        LispObject& operator *() const;

        LispObject* get() const;
        bool isNull() const;

        operator bool() const;

    private:
        LispObject* m_pPtr;
    };
}
