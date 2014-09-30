#pragma once

#include "lcpp/core/typeSystem/attribute.h"

namespace lcpp
{
    class LispObject;

    LCPP_API_CORE_CONT void attributeCheckAll(const StackPtr<LispObject>& pObject, const AttributeFlags& expectedFlags);
    LCPP_API_CORE_CONT void attributeCheckAny(const StackPtr<LispObject>& pObject, const AttributeFlags& expectedFlags);
    LCPP_API_CORE_CONT void attributeCheckNone(const StackPtr<LispObject>& pObject, const AttributeFlags& expectedFlags);

    void attributeCheckFailed(const char* szMessage);
}

#include "lcpp/core/typeSystem/impl/attributeCheck.inl"
