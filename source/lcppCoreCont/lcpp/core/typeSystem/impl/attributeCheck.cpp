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
        LCPP_LogBlock("attributeCheckAll");

        EZ_ASSERT(!pObject.isNull(), "Invalid object pointer.");

        auto& actualFlags = object::getAttributes(pObject);

        if(actualFlags != expectedFlags)
        {
            ezStringBuilder theString;
            theString.Append("Attribute check failed, expected all flags of: ");
            expectedFlags.toString(theString);
            attributeCheckFailed(theString.GetData());
        }
    }

    LCPP_API_CORE_CONT void attributeCheckAny(const Ptr<LispObject>& pObject, const AttributeFlags& expectedFlags)
    {
        LCPP_LogBlock("attributeCheckAny");

        EZ_ASSERT(!pObject.isNull(), "Invalid object pointer.");

        auto& actualFlags = object::getAttributes(pObject);

        if((actualFlags.getFlags() & expectedFlags.getFlags()) == 0)
        {
            ezStringBuilder theString;
            theString.Append("Attribute check failed, expected any flags of: ");
            expectedFlags.toString(theString);
            attributeCheckFailed(theString.GetData());
        }
    }

    LCPP_API_CORE_CONT void attributeCheckNone(const Ptr<LispObject>& pObject, const AttributeFlags& unexpectedFlags)
    {
        LCPP_LogBlock("attributeCheckNone");

        EZ_ASSERT(!pObject.isNull(), "Invalid object pointer.");

        auto& actualFlags = object::getAttributes(pObject);

        if((actualFlags.getFlags() & unexpectedFlags.getFlags()) != 0)
        {
            ezStringBuilder theString;
            theString.Append("Attribute check failed, expected no flags of: ");
            unexpectedFlags.toString(theString);
            attributeCheckFailed(theString.GetData());
        }
    }

#endif
}
