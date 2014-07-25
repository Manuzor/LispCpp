#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace number
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Number, "number");

            return meta;
        }

        Ptr<LispObject> create(Integer_t value)
        {
            auto pAllocator = LispRuntime::instance()->allocator();

            auto sizeA = sizeof(LispObjectHeader);
            auto alignmentA = EZ_ALIGNMENT_OF(LispObjectHeader);

            auto sizeB = sizeof(Data);
            auto alignmentB = EZ_ALIGNMENT_OF(Data);

            auto allocationSize = sizeA + sizeB;
            auto alignment = ezMath::Max(alignmentA, alignmentB);

            auto pMem = pAllocator->Allocate(allocationSize, alignment);
            auto memSize = pAllocator->AllocatedSize(pMem);

            auto pInstance = new (pMem) LispObject(metaInfo());

            pInstance->m_number.m_type = NumberType::Integer;
            pInstance->m_number.m_integer = value;

            return pInstance;
        }
    }
}
