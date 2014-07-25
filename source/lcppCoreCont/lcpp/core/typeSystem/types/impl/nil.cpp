#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    class LispNilMeta
    {
    public:

        MetaInfo m_typeInfo;

        LispNilMeta() :
            m_typeInfo(Type::Nil, "nil")
        {
        }
    };

    Ptr<LispObject> LispNil::create()
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
