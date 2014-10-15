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
        static void scan(CollectableBase* pCollectable, GarbageCollectionContext* pGC)
        {
            static ezUInt32 uiScanCount(0);
            ++uiScanCount;

            auto pObject = reinterpret_cast<LispObject*>(pCollectable);
            typeCheck(pObject, Type::Environment);

            // Note: Symbols are not garbage collected.

            LispObject*& pParent = pObject->getData<Data>().getParent().get();
            pParent = pGC->addSurvivor(pParent);

            auto& table = detail::getTable(pObject);
            for (auto iter = table.GetIterator(); iter.IsValid(); ++iter)
            {
                auto& pValue = iter.Value();
                pValue = pGC->addSurvivor(pValue);
            }
        }

        static void destroy(CollectableBase* pCollectable)
        {
            Ptr<LispObject> pObject(static_cast<LispObject*>(pCollectable));
            typeCheck(pObject, Type::Environment);

            auto& table = pObject->getData<Data>().getTable();
            table.~HashTable();
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Environment);
                meta.setPrettyName("environment");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction, static_cast<ScanFunction_t>(&scan)));
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction, static_cast<DestructorFunction_t>(&destroy)));

                return meta;
            }(); // Note that this lambda is immediately called.
            return &meta;
        }

        Ptr<LispObject> create(StackPtr<LispObject> pParent,
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

        Ptr<LispObject> createTopLevel(StackPtr<LispObject> pName)
        {
            return create(LCPP_pNil, pName);
        }

        Ptr<LispObject> createAnonymous(StackPtr<LispObject> pParent)
        {
            return create(pParent, symbol::create("anonymous"));
        }

        //////////////////////////////////////////////////////////////////////////

        Ptr<LispObject> getName(Ptr<LispObject> pEnv)
        {
            typeCheck(pEnv, Type::Environment);

            return pEnv->getData<Data>().getName();
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

            return pEnv->getData<Data>().getParent();
        }

        void addBinding(Ptr<LispObject> pEnv,
                        Ptr<LispObject> pSymbol,
                        Ptr<LispObject> pValue)
        {
            typeCheck(pEnv, Type::Environment);
            typeCheck(pSymbol, Type::Symbol);

            auto& table = detail::getTable(pEnv);

            table[pSymbol.get()] = pValue.get();
        }

        ezResult setBinding(Ptr<LispObject> pEnv,
                            Ptr<LispObject> pSymbol,
                            Ptr<LispObject> pValue)
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

        ezResult getBinding(Ptr<LispObject> pEnv,
                            Ptr<LispObject> pSymbol,
                            Ptr<LispObject>& out_pValue)
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

        BindingLocation existsBinding(Ptr<LispObject> pEnv,
                                      Ptr<LispObject> pSymbol)
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
            HashTable& getTable(Ptr<LispObject> pEnv)
            {
                return pEnv->getData<Data>().getTable();
            }
        }
    }
}
