#pragma once

#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    bool isCallable(Ptr<LispObject> pObject);
    bool isBuiltin(Ptr<LispObject> pObject);
    bool isNameable(Ptr<LispObject> pObject);

    LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);
}

#include "lcpp/core/typeSystem/impl/objectUtils.inl"
