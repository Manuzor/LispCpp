#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/nil.h"

namespace lcpp
{
    namespace env
    {
        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pName, Ptr<LispObject> pParent)
        {
            LCPP_NOT_IMPLEMENTED;
        }

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispObject> pName)
        {
            return create(pName, LCPP_pNil);
        }

        LCPP_API_CORE_CONT Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent)
        {
            return create(LCPP_pNil, pParent);
        }

        //////////////////////////////////////////////////////////////////////////

        void add(ezInt32 indexOfKeyValuePair /*= -1*/)
        {
            LCPP_NOT_IMPLEMENTED;
        }

    };
}
