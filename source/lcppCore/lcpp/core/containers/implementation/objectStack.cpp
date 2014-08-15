#include "stdafx.h"
#include "lcpp/core/containers/objectStack.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/nil.h"

inline
lcpp::Ptr<lcpp::LispObjectStack>
lcpp::LispObjectStack::create()
{
    auto pAllocator = LispRuntime::instance()->allocator();
    auto pInstance = LCPP_NEW(pAllocator.get(), LispObjectStack)(pAllocator);
    pInstance->setNilElement(LCPP_NIL);
    return pInstance;
}
