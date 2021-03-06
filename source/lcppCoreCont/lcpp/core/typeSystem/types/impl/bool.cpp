#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"

#include "lcpp/core/typeSystem/types/string.h"

EZ_ON_GLOBAL_EVENT(LCPP_GLOBAL_EVENT_SHUTDOWN)
{
    lcpp::getGarbageCollector()->destroyStatic(LCPP_pTrue.get());
    lcpp::getGarbageCollector()->destroyStatic(LCPP_pFalse.get());
}

namespace lcpp
{
    namespace
    {
        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            typeCheck(pLhs, Type::True, Type::False);
            return pLhs == pRhs;
        }
    }

    namespace true_
    {
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                MetaInfo meta;
                meta.setType(Type::True);
                meta.setPrettyName("true-type");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction,
                                              static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create()
        {
            LCPP_LogBlock("true_::create");

            static Ptr<LispObject> pInstance;

            if (!pInstance)
            {
                auto pGarbageCollector = getGarbageCollector();
                pInstance = pGarbageCollector->createStatic<LispObject>(getMetaInfo());
            }

            return pInstance;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::True);

            return str::create("#t");
        }

    }

    namespace false_
    {
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::False);
                meta.setPrettyName("false-type");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction,
                                              static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create()
        {
            LCPP_LogBlock("false_::create");

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
            typeCheck(pObject, Type::False);

            return str::create("#f");
        }

    }
}
