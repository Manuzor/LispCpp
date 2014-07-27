#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"

namespace lcpp
{
    namespace env
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Environment, "environment");
            return meta;
        }

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pName,
                                                  Ptr<LispObject> pParent)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            auto pTable = LCPP_NEW(LCPP_pRuntime->allocator().get(), HashTable)();

            if (!pName)
            {
                pName = symbol::create("anonymous");
            }

            pInstance->getBody().m_env.initialize(pName, pParent, pTable);

            return pInstance;
        }

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispObject> pName)
        {
            return create(pName, LCPP_pNil);
        }

        LCPP_API_CORE_CONT Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent)
        {
            return create(LCPP_pNil, pParent);
        }

        //////////////////////////////////////////////////////////////////////////

        void add(ezInt32 indexOfKeyValuePair /*= -1*/)
        {
            LCPP_NOT_IMPLEMENTED;
        }

    };
}
