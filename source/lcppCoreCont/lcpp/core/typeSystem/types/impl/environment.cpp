#include "stdafx.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/environmentData.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/symbol.h"

namespace lcpp
{
    namespace env
    {
        static void scan(Ptr<CollectableBase> pCollectable, GarbageCollector::PatchablePointerArray& pointersToPatch)
        {
            auto pObject = pCollectable.cast<LispObject>();
            typeCheck(pObject, Type::Environment);

            auto pName = reinterpret_cast<Ptr<CollectableBase>&>(pObject->getData<Data>().getName());
            auto pParent = reinterpret_cast<Ptr<CollectableBase>&>(pObject->getData<Data>().getParent());

            auto& table = detail::getTable(pObject);

            for (auto iter = table.GetIterator(); iter.IsValid(); ++iter)
            {
                auto pKey = &reinterpret_cast<Ptr<CollectableBase>&>(const_cast<StackPtr<LispObject>&>(iter.Key()));
                auto pValue = &reinterpret_cast<Ptr<CollectableBase>&>(iter.Value());

                pointersToPatch.PushBack(pKey);
                pointersToPatch.PushBack(pValue);
            }
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Environment);
                meta.setPrettyName("environment");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction, &scan));

                return meta;
            }(); // Note that this lambda is immediately called.
            return &meta;
        }

        StackPtr<LispObject> create(StackPtr<LispObject> pParent,
                               StackPtr<LispObject> pName)
        {
            LCPP_LogBlock("env::create");

            if(!isNil(pParent)) typeCheck(pParent, Type::Environment);
            typeCheck(pName, Type::Symbol);

            auto pInstance = object::create<Data>(getMetaInfo());

            auto& data = pInstance->getData<Data>();

            new (data.m_pName) Ptr<LispObject>(pName);
            new (data.m_pParent) Ptr<LispObject>(pParent);
            new (data.m_table) HashTable();

            return pInstance;
        }

        StackPtr<LispObject> createTopLevel(StackPtr<LispObject> pName)
        {
            return create(LCPP_pNil, pName);
        }

        StackPtr<LispObject> createAnonymous(StackPtr<LispObject> pParent)
        {
            return create(pParent, symbol::create("anonymous"));
        }

        //////////////////////////////////////////////////////////////////////////

        StackPtr<LispObject> getName(StackPtr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            return pEnv->getData<Data>().getName();
        }

        StackPtr<LispObject> getQualifiedName(StackPtr<LispObject> pEnv)
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

        StackPtr<LispObject> getParent(StackPtr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            return pEnv->getData<Data>().getParent();
        }

        void addBinding(StackPtr<LispObject> pEnv,
                        StackPtr<LispObject> pSymbol,
                        StackPtr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            table[pSymbol] = pValue;
        }

        ezResult setBinding(StackPtr<LispObject> pEnv,
                            StackPtr<LispObject> pSymbol,
                            StackPtr<LispObject> pValue)
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

        ezResult getBinding(StackPtr<LispObject> pEnv,
                            StackPtr<LispObject> pSymbol,
                            StackPtr<LispObject>& out_pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            auto pResult = StackPtr<LispObject>();
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

        BindingLocation existsBinding(StackPtr<LispObject> pEnv,
                                      StackPtr<LispObject> pSymbol)
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
            HashTable& getTable(StackPtr<LispObject> pEnv)
            {
                return pEnv->getData<Data>().getTable();
            }
        }
    }
}
