#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace number
    {
        const MetaInfo& metaInfoInteger()
        {
            static auto meta = MetaInfo(Type::Integer, "integer");
            return meta;
        }

        const MetaInfo& metaInfoFloat()
        {
            static auto meta = MetaInfo(Type::Float, "float");
            return meta;
        }

        Ptr<LispObject> create(Integer_t value)
        {
            auto pInstance = LispObject::create<Integer_t>(metaInfoInteger());

            pInstance->getBody().m_integer = value;

            return pInstance;
        }

        Ptr<LispObject> create(Float_t value)
        {
            auto pInstance = LispObject::create<Float_t>(metaInfoFloat());

            pInstance->getBody().m_float = value;

            return pInstance;
        }
    }
}
