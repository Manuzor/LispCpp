#include "stdafx.h"

#include "lcpp/core/builtinFunctions_recursive.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/evaluator.h"

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::define(Ptr<Environment> pEnv,
                     Ptr<IEvaluator> pEvaluator,
                     Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'define' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();

    if(!pArgList->car()->is<SchemeSymbol>())
    {
        throw exceptions::InvalidSyntax("First argument to syntax 'define' must be a symbol!");
    }

    if(isNil(pArgList->cdr()))
    {
        throw exceptions::InvalidSyntax("Not enough arguments for syntax 'define'!");
    }

    if(!isNil(pArgList->cdr().cast<SchemeCons>()->cdr()))
    {
        throw exceptions::InvalidSyntax("Too many arguments for syntax 'define'!");
    }

    auto symbol = pArgList->car().cast<SchemeSymbol>();
    auto value = pArgList->cdr().cast<SchemeCons>()->car();

    value = pEvaluator->evalulate(value);

    pEnv->add(symbol, value);

    // If it is a function, give it its new name.
    if(value->is<SchemeFunction>())
    {
        value.cast<SchemeFunction>()->name() = symbol->value();
    }

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::lambda(Ptr<Environment> pEnv,
                     Ptr<IEvaluator> pEvaluator,
                     Ptr<SchemeObject> pArgs)
{
    std::function<void(Ptr<SchemeCons>)> checkArgNameList = [&](Ptr<SchemeCons> pCons){
        if(!pCons->car()->is<SchemeSymbol>())
        {
            throw exceptions::InvalidSyntax("Lambda argument list only accepts symbols!");
        }
        if(isNil(pCons->cdr())) { return; }

        EZ_ASSERT(pCons->cdr()->is<SchemeCons>(), "Invalid AST!");

        checkArgNameList(pCons->cdr().cast<SchemeCons>());
    };

    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Built-in function 'define' expects exactly 2 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();
    auto pArgNameList = pArgList->car();

    // Arglist must be either nil or cons
    if(!isNil(pArgNameList) && !pArgNameList->is<SchemeCons>())
    {
        throw exceptions::InvalidSyntax("A 'lambda' needs an argument list!");
    }

    if(!isNil(pArgNameList))
    {
        checkArgNameList(pArgNameList.cast<SchemeCons>());
    }

    auto pBody = pArgList->cdr();

    if(isNil(pBody))
    {
        throw exceptions::InvalidSyntax("Lambda needs to have a body!");
    }

    auto pBodyList = pBody.cast<SchemeCons>();

    return pEvaluator->factory()->createUserDefinedFunction(pEnv, pArgNameList, pBodyList);
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::syntax::if_(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidSyntax("Syntax 'if' expects exactly 3 arguments!");
    }

    auto pArgList = pArgs.cast<SchemeCons>();

    {
        ezUInt32 numArgs = 0;
        count(pArgList, numArgs);
        if(numArgs != 3)
        {
            ezStringBuilder builder;
            builder.AppendFormat("Expected 3 argument(s), got %u.", numArgs);
            throw exceptions::InvalidInput(builder.GetData());
        }
    }

    auto expression = pArgList->car();
    auto thenPart = pArgList->cdr().cast<SchemeCons>()->car();
    auto elsePart = pArgList->cdr().cast<SchemeCons>()->cdr().cast<SchemeCons>()->car();

    auto expressionResult = pEvaluator->evalulate(expression);

    if (!expressionResult->is<SchemeBool>())
    {
        throw exceptions::InvalidInput("Given expression in 'if' does not evaluate to a boolean value!");
    }

    if(isTrue(expressionResult))
    {
        return pEvaluator->evalulate(thenPart);
    }

    return pEvaluator->evalulate(elsePart);
}


// Built-in functions
//////////////////////////////////////////////////////////////////////////

#define LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(numArgsExpected)                           \
    if (isNil(pArgs))                                                                      \
    {                                                                                      \
    throw exceptions::InvalidInput("Expected " #numArgsExpected " argument, got none.");   \
    }

#define LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, numArgsExpected)                         \
    {                                                                                            \
        ezUInt32 numArgs = 0;                                                                    \
        count(pArgList, numArgs);                                                                \
        if(numArgs != numArgsExpected)                                                           \
        {                                                                                        \
            ezStringBuilder builder;                                                             \
            builder.AppendFormat("Expected " #numArgsExpected " argument(s), got %u.", numArgs); \
            throw exceptions::InvalidInput(builder.GetData());                                   \
        }                                                                                        \
    }

#define LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pObject, expectedType)    \
    if(!pObject->is<expectedType>())                               \
    {                                                              \
        ezStringBuilder message;                                   \
        message.AppendFormat("Expected type %s, got %s",           \
                              TypeInfo<expectedType>::type().name, \
                              pArgList->car()->type().name);       \
        throw exceptions::InvalidInput(message.GetData());         \
    }                                                              \

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::exit(Ptr<Environment> pEnv,
                    Ptr<IEvaluator> pEvaluator,
                    Ptr<SchemeObject> pArgs)
{
    ezInt32 status = 0;
    if(!isNil(pArgs) && pArgs->is<SchemeCons>() && pArgs.cast<SchemeCons>()->car()->is<SchemeInteger>())
    {
        auto tmp = pArgs.cast<SchemeCons>()->car().cast<SchemeInteger>()->value();
        status = ezInt32(tmp);
    }

    throw exceptions::Exit("Goodbye.", status);

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::dump(Ptr<Environment> pEnv,
                    Ptr<IEvaluator> pEvaluator,
                    Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);
    
    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    auto pToDump = pArgList->car();

    if(pToDump->is<SchemeFunction>())
    {
        return pEvaluator->factory()->createString(pToDump.cast<SchemeFunction>()->dump());
    }

    return pEvaluator->factory()->createString(pToDump->toString());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileOpen(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    {
        ezUInt32 numArgs = 0;
        count(pArgList, numArgs);
        if(numArgs < 1 || numArgs > 2)
        {
            ezStringBuilder builder;
            builder.AppendFormat("Expected 1 or 2 argument(s), got %u.", numArgs);
            throw exceptions::InvalidInput(builder.GetData());
        }
    }

    auto pFileNameOrObject = pArgList->car();
    auto pFileMode = pArgList->cdr();

    Ptr<SchemeFile> pFile;

    if(pFileNameOrObject->is<SchemeFile>())
    {
        pFile = pFileNameOrObject.cast<SchemeFile>();
    }

    if(pFileNameOrObject->is<SchemeString>())
    {
        // create the file object.
        auto pToLoad = pFileNameOrObject.cast<SchemeString>();
        pFile = pEvaluator->factory()->createFile(pToLoad->value());
    }

    if(isTrue(pFile->isOpen()))
    {
        throw exceptions::InvalidOperation("Files can not be re-opened.");
    }

    if(!isNil(pFileMode) && !pFileMode->is<SchemeString>())
    {
        throw exceptions::InvalidInput("Expected either nil or a string as second parameter!");
    }
    
    if(isFalse(pFile->open(pFileMode)))
    {
        // If opening failed, return nil.
        return SCHEME_NIL_PTR;
    }
    
    return pFile;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileIsOpen(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), SchemeFile);

    auto pFile = pArgList->car().cast<SchemeFile>();

    return pFile->isOpen();
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileClose(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), SchemeFile);

    auto pFile = pArgList->car().cast<SchemeFile>();

    pFile->close();

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileReadString(Ptr<Environment> pEnv, Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), SchemeFile);

    auto pFile = pArgList->car().cast<SchemeFile>();

    if (pFile->isOpen() != SCHEME_TRUE_PTR)
    {
        throw exceptions::InvalidOperation("Cannot read string from a closed file.");
    }

    return pEvaluator->factory()->createString(pFile->readString());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::add(Ptr<Environment> pEnv,
                   Ptr<IEvaluator> pEvaluator,
                   Ptr<SchemeObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 1 argument, got none.");
    }

    auto pArgList = pArgs.cast<SchemeCons>();
    SchemeInteger::Number_t iResult = 0;
    SchemeNumber::Number_t nResult = 0;

    bool integerOnly = true;

    while(true)
    {
        auto pArg = pArgList->car();
        
        if (pArg->is<SchemeInteger>())
        {
            iResult += pArg.cast<SchemeInteger>()->value();
        }
        else if (pArg->is<SchemeNumber>())
        {
            integerOnly = false;
            nResult += pArg.cast<SchemeNumber>()->value();
        }
        else
        {
            throw exceptions::InvalidInput("Invalid argument type for 'add'");
        }

        if(isNil(pArgList->cdr())) { break; }
        
        pArgList = pArgList->cdr().cast<SchemeCons>();
    }

    if (integerOnly)
    {
        return pEvaluator->factory()->createInteger(iResult);
    }

    return pEvaluator->factory()->createNumber(iResult + nResult);
}

#undef LCPP_BUILTIN_FUNCTION_CHECK_TYPE
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL
