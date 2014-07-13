#include "stdafx.h"

#include "lcpp/foundation/ptr.h"

#include "lcpp/core/typeSystem.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/recursionCounter.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

lcpp::Ptr<lcpp::LispSyntax>
lcpp::LispSyntax::create(Ptr<LispSymbol> pName)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispSyntax::typeInfo()
{
    static auto t = Type::create(Type::Flags::Abstract | Type::Flags::Callable | Type::Flags::Syntax,
                                 EZ_STRINGIZE(LispSyntax),
                                 "syntax",
                                 MemoryInfo(sizeof(LispSyntax),
                                            sizeof(LispSyntax)));

    return t;
}

//////////////////////////////////////////////////////////////////////////

lcpp::LispSyntax::LispSyntax(Ptr<LispSymbol> pName) :
    m_pName(pName)
{
}

bool
lcpp::LispSyntax::operator==(const LispObject& obj) const
{
    if (obj.is<LispSyntax>())
    {
        return *this == static_cast<const LispSyntax&>(obj);
    }
    return false;
}

bool
lcpp::LispSyntax::operator==(const LispSyntax& rhs) const
{
    // Identity comparison.
    return m_pName.get() == rhs.m_pName.get();
}

ezString
lcpp::LispSyntax::toString() const
{
    return m_pName->toString();
}

lcpp::Ptr<lcpp::LispSyntax_Builtin>
lcpp::LispSyntax_Builtin::create(Ptr<LispSymbol> pName, HandlerFuncPtr_t pHandler)
{
    EZ_REPORT_FAILURE("Not implemented.");
    return nullptr;
}

const lcpp::Type&
lcpp::LispSyntax_Builtin::typeInfo()
{
    static auto t = Type::create(Type::Flags::Callable | Type::Flags::Syntax,
                                 EZ_STRINGIZE(LispSyntax_Builtin),
                                 "syntax (built-in)",
                                 MemoryInfo(sizeof(LispSyntax_Builtin),
                                            EZ_ALIGNMENT_OF(LispSyntax_Builtin)));

    return t;
}

//////////////////////////////////////////////////////////////////////////

lcpp::LispSyntax_Builtin::LispSyntax_Builtin(Ptr<LispSymbol> pName,
                                                 HandlerFuncPtr_t pHandler) :
    LispSyntax(pName),
    m_pHandler(pHandler)
{
    EZ_ASSERT(m_pHandler, "Invalid syntax handler pointer!");
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispSyntax_Builtin::call(Ptr<LispRuntime> pRuntime,
                                 Ptr<Environment> pEnv,
                                 Ptr<LispObject> pUnevaluatedArgList)
{
    RecursionCounter counter(pRuntime);
    auto& handler = *m_pHandler;
    return (*m_pHandler)(pRuntime, pEnv, pUnevaluatedArgList);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::LispSyntax_Builtin::clone(ezAllocatorBase* pAllocator) const
{
    return LCPP_NEW(pAllocator, LispSyntax_Builtin)(m_pName, m_pHandler);
}
