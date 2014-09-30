#pragma once

// These helpers help the compiler choose the correct overload of the number::create function.

namespace lcpp
{
    class LispObject;

    namespace number
    {
        StackPtr<LispObject> create(FloatAlternative_t value);

        // Helper to help the compiler choose the Integer_t overload.
        template<typename T_Number>
        StackPtr<LispObject> create(T_Number value);
    }
}

#include "lcpp/core/typeSystem/types/impl/numberHelpers.inl"
