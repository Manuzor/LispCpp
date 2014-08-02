#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"

#include "lcpp/core/typeSystem/types/syntax_builtinFunctions.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

void lcpp::LispRuntimeState::registerBuiltIns()
{
    // Character macros.
    //////////////////////////////////////////////////////////////////////////
    reader::addCharacterMacro(this, symbol::create("("), lambda::builtin::create(m_pGlobalEnvironment, &reader::detail::readList));
    reader::addCharacterMacro(this, symbol::create("\""), lambda::builtin::create(m_pGlobalEnvironment, &reader::detail::readString));

    // Reader macros / syntax.
    //////////////////////////////////////////////////////////////////////////
    reader::addSyntax(this, symbol::create("quote"), syntax::builtin::create(&syntax::builtin::quote));

    // Macros.
    //////////////////////////////////////////////////////////////////////////
    

    // Builtin functions.
    //////////////////////////////////////////////////////////////////////////
    

}
