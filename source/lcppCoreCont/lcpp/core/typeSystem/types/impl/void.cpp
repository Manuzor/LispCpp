#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"

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

                auto pAllocator = LispRuntime::instance()->allocator();
                pInstance = LCPP_NEW(pAllocator.get(), LispObject)(meta);
            }

            return pInstance;
        }
    }

}
