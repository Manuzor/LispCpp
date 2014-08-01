#pragma once

#include "lcpp/core/typeSystem/attribute.h"

namespace lcpp
{
    class LispObject;

    LCPP_API_CORE_CONT void attributeCheckAll(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags);
    LCPP_API_CORE_CONT void attributeCheckAny(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags);
}

#include "lcpp/core/typeSystem/impl/attributeCheck.inl"
