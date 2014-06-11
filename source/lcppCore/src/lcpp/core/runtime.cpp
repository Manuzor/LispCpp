#include "stdafx.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/typeFactory.h"

lcpp::SchemeRuntime::SchemeRuntime()
{
}

void
lcpp::SchemeRuntime::initialize()
{
    m_pAllocator = defaultAllocator();
    m_pFactory = LCPP_NEW(m_pAllocator, TypeFactory)(this);
    
    m_pReader = LCPP_NEW(m_pAllocator, Reader)(this, Reader::CInfo());
    m_pEvaluator = LCPP_NEW(m_pAllocator, RecursiveEvaluator)(this);

    m_pSyntaxEnvironment = LCPP_NEW(m_pAllocator, Environment)("syntax", nullptr);
    m_pGlobalEnvironment = LCPP_NEW(m_pAllocator, Environment)("global", m_pSyntaxEnvironment);

    //////////////////////////////////////////////////////////////////////////
    
    m_pReader->initialize();
    m_pEvaluator->initialize();
}
