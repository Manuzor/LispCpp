#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/typeCheck.h"

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
            // TODO Allocate enough memory so the object and the ezString can live in the same block.

            auto pInstance = LispObject::create<Data>(metaInfo());

            auto pString = LCPP_NEW(LCPP_pRuntime->allocator().get(), String)(value);

            pInstance->getBody().m_symbol.initialize(pString);

            return pInstance;
        }

        const String& value(ezInt32 index /*= -1*/)
        {
            auto pObject = LCPP_pStack->get(index);
            typeCheck(pObject, Type::Symbol);

            return *pObject->getBody().m_symbol.m_pValue;
        }

    }
}
