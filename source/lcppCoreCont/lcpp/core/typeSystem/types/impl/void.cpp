#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

namespace lcpp
{
    namespace void_
    {
        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if (!pInstance)
            {
                static auto meta = MetaInfo(Type::Void, "void");

                auto pAllocator = defaultAllocator();
                pInstance = LCPP_NEW(pAllocator, LispObject)(meta);
            }

            return pInstance;
        }

        String toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Void);

            static auto theString = String("#v");

            return theString;
        }
    }

}
