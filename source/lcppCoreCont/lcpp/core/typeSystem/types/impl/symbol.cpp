#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/symbolData.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/containers/instanceTable.h"
#include "lcpp/core/typeSystem/objectData.h"

#include <Foundation/Memory/MemoryUtils.h>
#include "lcpp/core/typeSystem/types/string.h"


#ifndef VerboseDebugMessage
// Enable this to allow verbose debug messages
#define VerboseDebugMessage Debug
#endif

namespace lcpp
{
    namespace symbol
    {
        static void destroy(CollectableBase* pCollectable)
        {
            auto pSymbol = static_cast<LispObject*>(pCollectable);
            typeCheck(pSymbol, Type::Symbol);

            pSymbol->getData<Data>().m_string.~String();
        }

        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            typeCheck(pLhs, Type::Symbol);
            return pLhs == pRhs;
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Symbol);
                meta.setPrettyName("symbol");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction,
                                              static_cast<DestructorFunction_t>(&destroy)));
                meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction, static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(const String& value)
        {
            LCPP_LogBlock("symbol::create");

            static auto symbolTable = InsanceTable<String>(&createNew);

            return symbolTable.get(value);
        }

        Ptr<LispObject> createNew(const String& value)
        {
            auto pInstance = object::createStatic<Data>(getGarbageCollector(), getMetaInfo());

            pInstance->getData<Data>().m_string = value;

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return pObject->getData<Data>().m_string;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            auto& sData = pObject->getData<Data>().m_string;
            return str::create(sData.GetData(), sData.GetElementCount());
        }

    }
}
