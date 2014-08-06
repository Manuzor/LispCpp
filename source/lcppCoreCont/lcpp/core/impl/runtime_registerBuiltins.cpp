#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"

#include "lcpp/core/typeSystem/objectUtils.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"

#include "lcpp/core/functionUtils/signature.h"

#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME


#define LCPP_AddCharacterMacro(szName, pFunction, signature) \
    reader::addCharacterMacro(this, symbol::create(szName), lambda::builtin::create(m_pGlobalEnvironment, pFunction, signature))

#define LCPP_AddSyntax(szName, pFunction, signature) \
    reader::addSyntax(this, symbol::create(szName), syntax::builtin::create(pFunction, signature))

#define LCPP_AddMacro(...)

#define LCPP_AddBuiltin(szName, pFunction, signature)                                        \
    do {                                                                                     \
        auto pName = symbol::create(szName);                                                 \
        auto pBuiltin = lambda::builtin::create(m_pGlobalEnvironment, pFunction, signature); \
        env::addBinding(m_pGlobalEnvironment, pName, pBuiltin);                              \
        if(!hasName(pBuiltin)) { setName(pBuiltin, pName); }                                 \
    } while(false)

void lcpp::LispRuntimeState::registerBuiltIns()
{
    // Character macros.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddCharacterMacro("(", &reader::detail::readList, Signature::create(1));
    LCPP_AddCharacterMacro("\"", &reader::detail::readString, Signature::create(1));

    // Reader macros / syntax.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddSyntax("quote", &syntax::builtin::quote, Signature::create(1));
    LCPP_AddSyntax("define", &syntax::builtin::define, Signature::create(2));

    // Macros.
    //////////////////////////////////////////////////////////////////////////


    // Builtin functions.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddBuiltin("+", &lambda::builtin::add, Signature::createVarArg());

}

#undef LCPP_AddBuiltin
#undef LCPP_AddMacro
#undef LCPP_AddSyntax
#undef LCPP_AddCharacterMacro
