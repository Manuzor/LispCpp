#include "stdafx.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/runtime.h"

lcpp::TypeFactory::TypeFactory() :
    m_symbols(LispRuntime::instance()->allocator().get()),
    m_integers(LispRuntime::instance()->allocator().get())
{
}

lcpp::TypeFactory::~TypeFactory()
{
    m_integers.Clear();
    m_symbols.Clear();
}
