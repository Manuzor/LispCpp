#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    Ptr<LispObject>
    LispObject::create()
    {
        static auto szMessage = "Cannot directly instantiate LispObject";
        EZ_ASSERT(false, szMessage);
        throw exceptions::InvalidOperation(szMessage);
    }
}
