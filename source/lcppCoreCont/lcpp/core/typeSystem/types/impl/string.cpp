#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

namespace lcpp
{
    namespace str
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::String, "string");

            return meta;
        }

        Ptr<LispObject> create(const String& value)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            new (pInstance->m_string.m_pRawData) String(value);

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            return pObject->m_string.getValue();
        }

        String toString(Ptr<LispObject> pObject)
        {
            LCPP_NOT_IMPLEMENTED;
        }

    }
}
