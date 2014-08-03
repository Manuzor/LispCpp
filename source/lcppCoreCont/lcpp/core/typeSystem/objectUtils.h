#pragma once

#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    bool isCallable(Ptr<LispObject> pObject);
    bool isBuiltin(Ptr<LispObject> pObject);
    bool isNameable(Ptr<LispObject> pObject);

    LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

    LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

    LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pObject);
    LCPP_API_CORE_CONT void setName(Ptr<LispObject> pObject, Ptr<LispObject> pName);
    LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pObject);
}

#include "lcpp/core/typeSystem/impl/objectUtils.inl"
