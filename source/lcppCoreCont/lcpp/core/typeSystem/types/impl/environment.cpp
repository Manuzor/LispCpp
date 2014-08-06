#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

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
            auto pInstance = object::create<Data>(metaInfo());

            auto& data = pInstance->m_env;

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

        Ptr<LispObject> getName(Ptr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            return pEnv->m_env.getName();
        }

        Ptr<LispObject> getQualifiedName(Ptr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            auto name = ezStringBuilder();

            while(true)
            {
                name.Prepend(symbol::getValue(getName(pEnv)).GetData());
                pEnv = getParent(pEnv);
                if(isNil(pEnv))
                {
                    break;
                }
                name.Prepend('/');
            }

            return str::create(name);
        }

        Ptr<LispObject> getParent(Ptr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            return pEnv->m_env.getParent();
        }

        void addBinding(Ptr<LispObject> pEnv,
                        Ptr<LispObject> pSymbol,
                        Ptr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            table[pSymbol] = pValue;
        }

        ezResult setBinding(Ptr<LispObject> pEnv,
                            Ptr<LispObject> pSymbol,
                            Ptr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            if(table.KeyExists(pSymbol))
            {
                table[pSymbol] = pValue;
                return EZ_SUCCESS;
            }

            auto pParent = getParent(pEnv);
            
            if(!isNil(pParent))
            {
                return setBinding(pParent, pSymbol, pValue);
            }
            
            return EZ_FAILURE;
        }

        ezResult getBinding(Ptr<LispObject> pEnv,
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
                return env::getBinding(pParent, pSymbol, out_pValue);
            }
            
            return EZ_FAILURE;
        }

        BindingLocation existsBinding(Ptr<LispObject> pEnv,
                                      Ptr<LispObject> pSymbol)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            if(table.KeyExists(pSymbol))
            {
                return BindingLocation::Local;
            }

            auto pParent = env::getParent(pEnv);

            if (!isNil(pParent))
            {
                auto location = existsBinding(pParent, pSymbol);
                if (location)
                {
                    return BindingLocation::Parent;
                }
            }

            return BindingLocation::None;
        }

        namespace detail
        {
            HashTable& getTable(Ptr<LispObject> pEnv)
            {
                return pEnv->m_env.getTable();
            }
        }
    }
}
