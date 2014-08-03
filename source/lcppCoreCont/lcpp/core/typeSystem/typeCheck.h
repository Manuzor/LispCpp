#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"

namespace lcpp
{
    class LispObject;

    /// \brief Checks whether the given \a pObject is of type \a expectedType.
    ///
    /// In case the type check fails, throws exceptions::TypeCheckFailed
    LCPP_API_CORE_CONT void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType);

    LCPP_API_CORE_CONT void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType1, const Type& expectedType2);
}

#include "lcpp/core/typeSystem/impl/typeCheck.inl"
