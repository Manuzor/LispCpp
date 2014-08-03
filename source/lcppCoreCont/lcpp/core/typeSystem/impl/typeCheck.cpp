#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

#include "lcpp/core/exceptions/typeCheckFailedException.h"

namespace lcpp
{
#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK)

    void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType)
    {
        EZ_ASSERT_ALWAYS(pObject, "Invalid object pointer.");

        auto& actualType = pObject->getType();
        if(actualType != expectedType)
        {
            auto message = ezStringBuilder();
            message.Format("Type check failed, expected '%s' got '%s'.",
                            expectedType.toString(),
                            actualType.toString());
            LCPP_THROW(exceptions::TypeCheckFailed(message.GetData()));
        }
    }

    void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType1, const Type& expectedType2)
    {
        EZ_ASSERT_ALWAYS(pObject, "Invalid object pointer.");

        auto& actualType = pObject->getType();
        if(actualType != expectedType1 && actualType != expectedType2)
        {
            auto message = ezStringBuilder();
            message.Format("Type check failed, expected '%s' or '%s' got '%s'.",
                           expectedType1.toString(),
                           expectedType2.toString(),
                           actualType.toString());
            LCPP_THROW(exceptions::TypeCheckFailed(message.GetData()));
        }
    }

#endif
}
