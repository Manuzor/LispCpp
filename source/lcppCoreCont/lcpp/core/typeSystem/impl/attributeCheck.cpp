#include "stdafx.h"
#include "lcpp/core/typeSystem/attributeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/metaInfo.h"

namespace lcpp
{
#if EZ_ENABLED(LCPP_RUNTIME_TYPE_CHECK)

    LCPP_API_CORE_CONT void attributeCheckAll(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        EZ_ASSERT(pObject, "Invalid object pointer.");

        auto& actualFlags = object::getMetaInfo(pObject).getAttributes();

        if(actualFlags != expectedFlags)
        {
            attributeCheckFailed("Invalid attribute flags.");
        }
    }

    LCPP_API_CORE_CONT void attributeCheckAny(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        EZ_ASSERT(pObject, "Invalid object pointer.");

        auto& actualFlags = object::getMetaInfo(pObject).getAttributes();

        if((actualFlags.getFlags() & expectedFlags.getFlags()) == 0)
        {
            attributeCheckFailed("Invalid attribute flags.");
        }
    }

    LCPP_API_CORE_CONT void attributeCheckNone(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        EZ_ASSERT(pObject, "Invalid object pointer.");

        auto& actualFlags = object::getMetaInfo(pObject).getAttributes();

        if((actualFlags.getFlags() & expectedFlags.getFlags()) != 0)
        {
            attributeCheckFailed("Invalid attribute flags.");
        }
    }

#endif
}
