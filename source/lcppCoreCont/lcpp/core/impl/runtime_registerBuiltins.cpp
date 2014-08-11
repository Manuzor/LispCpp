#include "stdafx.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/reader.h"

#include "lcpp/core/typeSystem/object.h"

#include "lcpp/core/typeSystem/types/symbol.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/lambda_builtin.h"

#include "lcpp/core/functionUtils/signature.h"

#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/bool.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/void.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME


#define LCPP_AddGlobalVariable(szName, pValue) \
    env::addBinding(m_pGlobalEnvironment, symbol::create(szName), pValue)

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
        if(!object::hasName(pBuiltin)) { object::setName(pBuiltin, pName); }                 \
    } while(false)

void lcpp::LispRuntimeState::registerBuiltIns()
{
    // Global objects.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddGlobalVariable("#t", LCPP_pTrue);
    LCPP_AddGlobalVariable("#f", LCPP_pFalse);
    LCPP_AddGlobalVariable("null", LCPP_pNil);
    LCPP_AddGlobalVariable("#v", LCPP_pVoid);

    // Character macros.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddCharacterMacro("(", &reader::detail::readList, Signature::create(1));
    LCPP_AddCharacterMacro("\"", &reader::detail::readString, Signature::create(1));

    // Reader macros / syntax.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddSyntax("quote", &syntax::builtin::quote, Signature::create(1));
    LCPP_AddSyntax("begin", &syntax::builtin::begin, Signature::createVarArg());
    LCPP_AddSyntax("if", &syntax::builtin::if_, Signature::create(3));
    LCPP_AddSyntax("define", &syntax::builtin::define, Signature::createVarArg(2));
    LCPP_AddSyntax("set!", &syntax::builtin::set, Signature::createVarArg(2));
    LCPP_AddSyntax("lambda", &syntax::builtin::lambda, Signature::createVarArg(2));
    LCPP_AddSyntax("and", &syntax::builtin::and, Signature::createVarArg());
    LCPP_AddSyntax("or", &syntax::builtin::or, Signature::createVarArg());

    // Macros.
    //////////////////////////////////////////////////////////////////////////


    // Builtin functions.
    //////////////////////////////////////////////////////////////////////////
    LCPP_AddBuiltin("+", &lambda::builtin::add, Signature::createVarArg());
    LCPP_AddBuiltin("-", &lambda::builtin::subtract, Signature::createVarArg(1));
    LCPP_AddBuiltin("*", &lambda::builtin::multiply, Signature::createVarArg());
    LCPP_AddBuiltin("/", &lambda::builtin::divide, Signature::createVarArg(1));
    LCPP_AddBuiltin("%", &lambda::builtin::modulo, Signature::create(2));

    LCPP_AddBuiltin(">",  &lambda::builtin::greaterThan, Signature::createVarArg(2));
    LCPP_AddBuiltin(">=", &lambda::builtin::greaterThanOrEqual, Signature::createVarArg(2));
    LCPP_AddBuiltin("=",  &lambda::builtin::equal, Signature::createVarArg(2));
    LCPP_AddBuiltin("<",  &lambda::builtin::lowerThan, Signature::createVarArg(2));
    LCPP_AddBuiltin("<=", &lambda::builtin::lowerThanOrEqual, Signature::createVarArg(2));

    LCPP_AddBuiltin("read", &lambda::builtin::read, Signature::create(1));

}

#undef LCPP_AddGlobalVariable
#undef LCPP_AddBuiltin
#undef LCPP_AddMacro
#undef LCPP_AddSyntax
#undef LCPP_AddCharacterMacro
