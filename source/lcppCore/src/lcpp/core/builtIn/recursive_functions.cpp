#include "stdafx.h"

#include "lcpp/core/builtIn/recursive_functions.h"
#include "lcpp/core/typeSystem.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/printer.h"

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

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
lcpp::builtin::exit(Ptr<SchemeRuntime> pRuntime,
                    Ptr<Environment> pEnv,
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
lcpp::builtin::dump(Ptr<SchemeRuntime> pRuntime,
                    Ptr<Environment> pEnv,
                    Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);
    
    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    auto pToDump = pArgList->car();

    if(pToDump->is<SchemeFunction>())
    {
        return pRuntime->factory()->createString(pToDump.cast<SchemeFunction>()->dump());
    }

    return pRuntime->factory()->createString(pToDump->toString());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::read(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), SchemeString);

    return pRuntime->reader()->read(pArgList->car().cast<SchemeString>()->value());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::eval(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    return pRuntime->evaluator()->evalulate(pEnv, pArgList->car());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::print(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
{
    static Printer printer;

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    printer.print(pArgs);

    return SCHEME_VOID_PTR;
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileOpen(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
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
        pFile = pRuntime->factory()->createFile(pToLoad->value());
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
lcpp::builtin::fileIsOpen(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<SchemeCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), SchemeFile);

    auto pFile = pArgList->car().cast<SchemeFile>();

    return pFile->isOpen();
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::fileClose(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
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
lcpp::builtin::fileReadString(Ptr<SchemeRuntime> pRuntime, Ptr<Environment> pEnv, Ptr<SchemeObject> pArgs)
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

    return pRuntime->factory()->createString(pFile->readString());
}

lcpp::Ptr<lcpp::SchemeObject>
lcpp::builtin::add(Ptr<SchemeRuntime> pRuntime,
                   Ptr<Environment> pEnv,
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
        return pRuntime->factory()->createInteger(iResult);
    }

    return pRuntime->factory()->createNumber(iResult + nResult);
}

#undef LCPP_BUILTIN_FUNCTION_CHECK_TYPE
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL
