#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/runtime.h"

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
            // TODO Allocate enough memory so the object and the ezString can live in the same block.

            auto pInstance = LispObject::create<Data>(metaInfo());

            auto pString = LCPP_NEW(LCPP_pRuntime->allocator().get(), String)(value);

            pInstance->getBody().m_symbol.initialize(pString);

            return pInstance;
        }
}
}
