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
            auto pInstance = LispObject::create<Data>(metaInfo());

            auto& data = pInstance->getBody().m_number;

            // Initialization
            data.m_type = NumberType::Integer;
            data.m_integer = value;

            return pInstance;
        }
    }
}
