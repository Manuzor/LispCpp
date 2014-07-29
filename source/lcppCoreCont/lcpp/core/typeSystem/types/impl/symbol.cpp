#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include <Foundation/Memory/MemoryUtils.h>

namespace lcpp
{
    namespace symbol
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Symbol, "symbol");
            return meta;
        }

        Ptr<LispObject> create(const String& value)
        {
            auto pSymbolTable = LCPP_pRuntime->instanceTables().m_pSymbolTable;

            return pSymbolTable->get(value);
        }

        Ptr<LispObject> createNew(const String& value)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            new (pInstance->m_symbol.m_pRawData) String(value);

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return pObject->m_symbol.getValue();
        }

    }
}
