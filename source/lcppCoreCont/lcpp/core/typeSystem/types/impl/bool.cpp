#include "stdafx.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"

#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace true_
    {
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                MetaInfo meta;
                meta.setType(Type::True);
                meta.setPrettyName("true-type");

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

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::True);

            static auto pString = str::create("#t");

            return pString;
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

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::False);

            static auto pString = str::create("#f");

            return pString;
        }

    }
}
