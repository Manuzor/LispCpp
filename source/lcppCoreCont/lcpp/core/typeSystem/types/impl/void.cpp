#include "stdafx.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace void_
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Void);
                meta.setPrettyName("void");

                return meta;
            }(); // Note that this lambda is immediately called.

            return meta;
        }

        Ptr<LispObject> create()
        {
            static auto pInstance = Ptr<LispObject>();

            if (!pInstance)
            {
                static LispObject instance(metaInfo());
                pInstance = &instance;
            }

            return pInstance;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Void);

            static auto pString = str::create("#v");

            return pString;
        }

    }

}
