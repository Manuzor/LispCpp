#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"

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
            EZ_REPORT_FAILURE(message.GetData());
        }
    }

#endif
}
