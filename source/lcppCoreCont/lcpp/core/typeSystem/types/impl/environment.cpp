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

        Ptr<LispObject> create(Ptr<LispObject> pName,
                               Ptr<LispObject> pParent)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            auto& data = pInstance->getBody().m_env;

            new (data.m_pName) Ptr<LispObject>(pName);
            new (data.m_pParent) Ptr<LispObject>(pParent);
            new (data.m_table) HashTable();

            return pInstance;
        }

        Ptr<LispObject> createTopLevel(Ptr<LispObject> pName)
        {
            return create(pName, LCPP_pNil);
        }

        Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent)
        {
            return create(symbol::create("anonymous"), pParent);
        }

        //////////////////////////////////////////////////////////////////////////

        void add(Ptr<LispObject> pEnv,
                 Ptr<LispObject> pSymbol,
                 Ptr<LispObject> pValue)
        {
            LCPP_NOT_IMPLEMENTED;
        }
    };
}
