#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace nil
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Nil);
                meta.setPrettyName("nil");

                return meta;
            }(); // Note that this lambda is immediately called.

            return meta;
        }

        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if (!pInstance)
            {
                auto pGarbageCollector = getGarbageCollector();
                pInstance = pGarbageCollector->create<LispObject>();
                pInstance->m_pMetaInfo = &metaInfo();
            }

            return pInstance;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Nil);

            static auto pString = str::create("()");

            return pString;
        }

    }
}
