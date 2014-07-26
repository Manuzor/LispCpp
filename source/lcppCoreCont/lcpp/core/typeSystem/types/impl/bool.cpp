#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
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

                auto pAllocator = LispRuntime::instance()->allocator();
                pInstance = LCPP_NEW(pAllocator.get(), LispObject)(meta);
            }

            return pInstance;
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

                auto pAllocator = LispRuntime::instance()->allocator();
                pInstance = LCPP_NEW(pAllocator.get(), LispObject)(meta);
            }

            return pInstance;
        }
    }
}
