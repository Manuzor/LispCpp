#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"


namespace lcpp
{
#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK)

    void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType)
    {
        LCPP_LogBlock("typeCheck");
        LCPP_AssertObjectIsAlive(pObject.get());

        EZ_ASSERT_ALWAYS(!pObject.isNull(), "Invalid object pointer.");

        auto& actualType = object::getType(pObject);
        if(actualType != expectedType)
        {
            auto message = ezStringBuilder();
            message.Format("Type check failed, expected '%s' got '%s'.",
                            expectedType.toString(),
                            actualType.toString());
            typeCheckFailed(message.GetData());
        }
    }

    void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType1, const Type& expectedType2)
    {
        LCPP_LogBlock("typeCheck");
        LCPP_AssertObjectIsAlive(pObject.get());

        EZ_ASSERT_ALWAYS(!pObject.isNull(), "Invalid object pointer.");

        auto& actualType = object::getType(pObject);
        if(actualType != expectedType1 && actualType != expectedType2)
        {
            auto message = ezStringBuilder();
            message.Format("Type check failed, expected '%s' or '%s' got '%s'.",
                           expectedType1.toString(),
                           expectedType2.toString(),
                           actualType.toString());
            typeCheckFailed(message.GetData());
        }
    }


#endif
}
