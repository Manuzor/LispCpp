#include "stdafx.h"
#include "lcpp/core/objectStack.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/nil.h"

lcpp::Ptr<lcpp::LispObjectStack>
lcpp::LispObjectStack::create(const ezString& name)
{
    return LCPP_NEW(LispRuntime::instance()->allocator().get(), LispObjectStack)(name);
}

lcpp::LispObjectStack::LispObjectStack(const ezString& name) :
    m_name(name),
    m_stack(LispRuntime::instance()->allocator().get())
{
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispObjectStack::get(ezInt32 relativeIndex)
{
    if(empty()) { return LCPP_NIL; }

    auto index = convertToAbsolute(relativeIndex);
    if(index > top())
    {
        // Invalid index (out of bounds)
        return LCPP_NIL;
    }

    return m_stack[index];
}
