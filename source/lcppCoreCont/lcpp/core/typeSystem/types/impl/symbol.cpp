#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/symbolData.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/containers/instanceTable.h"
#include "lcpp/core/typeSystem/objectData.h"

#include <Foundation/Memory/MemoryUtils.h>
#include "lcpp/core/typeSystem/types/string.h"

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
            static auto symbolTable = InsanceTable<String>(&createNew);

            return symbolTable.get(value);
        }

        Ptr<LispObject> createNew(const String& value)
        {
            auto pInstance = object::create<Data>(metaInfo());

            new (pInstance->m_symbol.m_pRawData) String(value);

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return pObject->m_symbol.getValue();
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return str::create(getValue(pObject).GetData());
        }

    }
}
