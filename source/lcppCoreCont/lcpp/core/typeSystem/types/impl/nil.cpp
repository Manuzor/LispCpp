#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    Ptr<LispObject> nil::create()
    {
        static auto pInstance = Ptr<LispObject>();

        if (!pInstance)
        {
            static auto meta = MetaInfo(Type::Nil, "nil");

            auto pAllocator = LispRuntime::instance()->allocator();
            auto pMem = pAllocator->Allocate(sizeof(LispObject), sizeof(LispObject));
            pInstance = new (pMem) LispObject(meta);
        }

        return pInstance;
    }
}
