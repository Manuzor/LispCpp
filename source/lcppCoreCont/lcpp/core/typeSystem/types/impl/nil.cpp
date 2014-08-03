#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace nil
    {
        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if (!pInstance)
            {
                static auto meta = MetaInfo(Type::Nil, "nil");

                auto pAllocator = defaultAllocator();
                pInstance = LCPP_NEW(pAllocator, LispObject)(meta);
            }

            return pInstance;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Nil);

            static auto pString = str::create("()");

            return pString;
        }
    }
}
