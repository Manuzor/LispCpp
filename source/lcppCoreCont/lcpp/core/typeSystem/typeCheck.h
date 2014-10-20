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

    void typeCheckFailed(const char* szMessage);

    extern ezUInt32 g_typeCheckOverride;

    struct DisableTypeCheckForScope
    {
        DisableTypeCheckForScope() { ++g_typeCheckOverride; }
        ~DisableTypeCheckForScope(){ --g_typeCheckOverride; }

        EZ_DISALLOW_COPY_AND_ASSIGN(DisableTypeCheckForScope);
    };
}

#include "lcpp/core/typeSystem/impl/typeCheck.inl"

#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK)
#define LCPP_DisableTypeCheckForScope ::lcpp::DisableTypeCheckForScope _disableTypeCheck_ ## EZ_SOURCE_LINE;
#else
#define LCPP_DisableTypeCheckForScope
#endif

