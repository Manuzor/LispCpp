#include "stdafx.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"

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

            pInstance->m_integer = value;

            return pInstance;
        }

        Ptr<LispObject> create(Float_t value)
        {
            auto pInstance = LispObject::create<Float_t>(metaInfoFloat());

            pInstance->m_float = value;

            return pInstance;
        }

        Integer_t getInteger(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Integer);

            return pObject->m_integer;
        }

        Float_t getFloat(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Float);

            return pObject->m_float;
        }

        String toString(Ptr<LispObject> pObject)
        {
            LCPP_NOT_IMPLEMENTED;
        }

    }
}
