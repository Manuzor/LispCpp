#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace true_
    {
        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if(!pInstance)
            {
                static auto meta = MetaInfo(Type::True, "true-type");

                auto pAllocator = defaultAllocator();
                pInstance = LCPP_NEW(pAllocator, LispObject)(meta);
            }

            return pInstance;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::True);

            static auto pString = str::create("#t");

            return pString;
        }

    }

    namespace false_
    {
        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if(!pInstance)
            {
                static auto meta = MetaInfo(Type::False, "false-type");

                auto pAllocator = defaultAllocator();
                pInstance = LCPP_NEW(pAllocator, LispObject)(meta);
            }

            return pInstance;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::False);

            static auto pString = str::create("#f");

            return pString;
        }

    }
}
