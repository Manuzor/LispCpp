#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/attribute.h"

#include "lcpp/core/exceptions/typeCheckFailedException.h"

namespace lcpp
{
#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK)

    LCPP_API_CORE_CONT void attributeCheckAll(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        EZ_ASSERT(pObject, "Invalid object pointer.");

        auto& actualFlags = pObject->getMetaInfo().getAttributes();

        if(actualFlags != expectedFlags)
        {
            LCPP_THROW(exceptions::TypeCheckFailed, "Invalid attribute flags.");
        }
    }

    LCPP_API_CORE_CONT void attributeCheckAny(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        EZ_ASSERT(pObject, "Invalid object pointer.");

        auto& actualFlags = pObject->getMetaInfo().getAttributes();

        if((actualFlags.getFlags() & expectedFlags.getFlags()) == 0)
        {
            LCPP_THROW(exceptions::TypeCheckFailed, "Invalid attribute flags.");
        }
    }

#endif
}
