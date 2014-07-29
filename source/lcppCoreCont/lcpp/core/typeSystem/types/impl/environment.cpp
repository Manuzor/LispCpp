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

        Ptr<LispObject> getName(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Environment);

            return pObject->getBody().m_env.getName();
        }

        Ptr<LispObject> getParent(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Environment);

            return pObject->getBody().m_env.getParent();
        }

        void add(Ptr<LispObject> pEnv,
                 Ptr<LispObject> pSymbol,
                 Ptr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            table[pSymbol] = pValue;
        }

        ezResult get(Ptr<LispObject> pEnv,
                     Ptr<LispObject> pSymbol,
                     Ptr<LispObject>& out_pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            auto pResult = Ptr<LispObject>();
            if(table.TryGetValue(pSymbol, pResult))
            {
                out_pValue = pResult;
                return EZ_SUCCESS;
            }

            auto pParent = env::getParent(pEnv);
            
            if(!isNil(pParent))
            {
                return env::get(pParent, pSymbol, out_pValue);
            }
            
            return EZ_FAILURE;
        }

        namespace detail
        {
            HashTable& getTable(Ptr<LispObject> pObject)
            {
                return pObject->getBody().m_env.getTable();
            }
        }
    }
}
