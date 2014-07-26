#pragma once

namespace lcpp
{
    class Type;
    class LispObject;

    /// \brief Checks whether the given \a pObject is of type \a expectedType.
    ///
    /// Raises an assert when LCPP_RUNTIME_TYPE_CHECK
    LCPP_API_CORE_CONT void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType);
}

#include "lcpp/core/typeSystem/impl/typeCheck.inl"
