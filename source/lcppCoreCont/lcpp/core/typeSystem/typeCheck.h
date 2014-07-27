#pragma once

namespace lcpp
{
    class Type;
    class LispObject;

    /// \brief Checks whether the given \a pObject is of type \a expectedType.
    ///
    /// In case the type check fails, throws exceptions::TypeCheckFailed
    LCPP_API_CORE_CONT void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType);
}

#include "lcpp/core/typeSystem/impl/typeCheck.inl"
