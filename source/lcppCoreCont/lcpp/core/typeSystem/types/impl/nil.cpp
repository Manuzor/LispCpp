#include "stdafx.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/types/string.h"


EZ_ON_GLOBAL_EVENT(LCPP_GLOBAL_EVENT_SHUTDOWN)
{
    lcpp::getGarbageCollector()->destroyStatic(LCPP_pNil.get());
}

namespace lcpp
{
    namespace nil
    {
        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            typeCheck(pLhs, Type::Nil);
            return pLhs == pRhs;
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Nil);
                meta.setPrettyName("nil");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction,
                                              static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create()
        {
            LCPP_LogBlock("nil::create");

            static Ptr<LispObject> pInstance;

            if (pInstance.isNull())
            {
                auto pGarbageCollector = getGarbageCollector();
                pInstance = pGarbageCollector->createStatic<LispObject>(getMetaInfo());
            }

            return pInstance;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Nil);

            return str::create("()");
        }

    }
}
