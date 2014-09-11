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
        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Symbol);
                meta.setPrettyName("symbol");

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
            auto pInstance = object::create<Data>(getMetaInfo());

            new (pInstance->getData<Data>().m_pRawData) String(value);

            return pInstance;
        }

        const String& getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return pObject->getData<Data>().getValue();
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::Symbol);

            return str::create(getValue(pObject).GetData());
        }

    }
}
