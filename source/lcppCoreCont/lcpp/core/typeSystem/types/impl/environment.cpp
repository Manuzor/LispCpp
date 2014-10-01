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
        static void scan(CollectableBase* pCollectable, GarbageCollector::PatchablePointerArray& pointersToPatch)
        {
            Ptr<LispObject> pObject(static_cast<LispObject*>(pCollectable));
            typeCheck(pObject, Type::Environment);

            // Note: Symbols are not garbage collected.

            auto ppParent = &reinterpret_cast<CollectableBase*&>(pObject->getData<Data>().getParent().get());
            pointersToPatch.PushBack(ppParent);

            auto& table = detail::getTable(pObject);
            for (auto iter = table.GetIterator(); iter.IsValid(); ++iter)
            {
                auto ppValue = &reinterpret_cast<CollectableBase*&>(iter.Value());
                pointersToPatch.PushBack(ppValue);
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

            new (data.m_pName) Ptr<LispObject>(pName.get());
            new (data.m_pParent) Ptr<LispObject>(pParent);

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

            table[pSymbol.get()] = pValue.get();
        }

        ezResult setBinding(StackPtr<LispObject> pEnv,
                            StackPtr<LispObject> pSymbol,
                            StackPtr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            if(table.KeyExists(pSymbol.get()))
            {
                table[pSymbol.get()] = pValue.get();
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

            LispObject* pLookupResult(nullptr);
            if(table.TryGetValue(pSymbol.get(), pLookupResult))
            {
                out_pValue = pLookupResult;
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

            if(table.KeyExists(pSymbol.get()))
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
