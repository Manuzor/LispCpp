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
                              expectedType::typeInfo().name,       \
                              pObject->type().name);               \
        throw exceptions::InvalidInput(message.GetData());         \
    }                                                              \

#define LCPP_BUILTIN_FUNCTION_CHECK_TYPE_2(pObject, expectedType1, expectedType2) \
    if(!pObject->is<expectedType1>()                                \
    && !pObject->is<expectedType2>())                               \
    {                                                               \
        ezStringBuilder message;                                    \
        message.AppendFormat("Expected type %s or %s, got %s",      \
                              expectedType1::typeInfo().name,       \
                              expectedType1::typeInfo().name,       \
                              pArgList->car()->type().name);        \
        throw exceptions::InvalidInput(message.GetData());          \
    }                                                               \

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::exit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    ezInt32 status = 0;
    if(!isNil(pArgs) && pArgs->is<LispCons>() && pArgs.cast<LispCons>()->car()->is<LispInteger>())
    {
        auto tmp = pArgs.cast<LispCons>()->car().cast<LispInteger>()->value();
        status = ezInt32(tmp);
    }

    throw exceptions::Exit("Goodbye.", status);

    return LCPP_VOID;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::dump(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);
    
    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    auto pToDump = pArgList->car();

    if(pToDump->is<LispFunction>())
    {
        return LispString::create(pToDump.cast<LispFunction>()->dump());
    }

    return LispString::create(pToDump->toString());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::read(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispString);

    return LispRuntime::instance()->reader()->read(pArgList->car().cast<LispString>()->value());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::eval(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    return LispRuntime::instance()->evaluator()->evalulate(pEnv, pArgList->car());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::print(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    static Printer printer;

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);

    printer.print(pArgList->car());

    return LCPP_VOID;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::fileOpen(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

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

    Ptr<LispFile> pFile;

    if(pFileNameOrObject->is<LispFile>())
    {
        pFile = pFileNameOrObject.cast<LispFile>();
    }

    if(pFileNameOrObject->is<LispString>())
    {
        // create the file object.
        auto pToLoad = pFileNameOrObject.cast<LispString>();
        pFile = LispRuntime::instance()->factory()->createFile(pToLoad->value());
    }

    if(isTrue(pFile->isOpen()))
    {
        throw exceptions::InvalidOperation("Files can not be re-opened.");
    }

    if(!isNil(pFileMode) && !pFileMode->is<LispString>())
    {
        throw exceptions::InvalidInput("Expected either nil or a string as second parameter!");
    }
    
    if(isFalse(pFile->open(pFileMode)))
    {
        // If opening failed, return nil.
        return LCPP_NIL;
    }
    
    return pFile;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::fileIsOpen(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispFile);

    auto pFile = pArgList->car().cast<LispFile>();

    return pFile->isOpen();
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::fileClose(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispFile);

    auto pFile = pArgList->car().cast<LispFile>();

    pFile->close();

    return LCPP_VOID;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::fileReadString(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispFile);

    auto pFile = pArgList->car().cast<LispFile>();

    if (pFile->isOpen() != LCPP_TRUE)
    {
        throw exceptions::InvalidOperation("Cannot read string from a closed file.");
    }

    return LispString::create(pFile->readString());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::add(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 1 argument, got none.");
    }

    auto pArgList = pArgs.cast<LispCons>();
    LispInteger::Number_t iResult = 0;
    LispNumber::Number_t nResult = 0;

    bool integerOnly = true;

    while(true)
    {
        auto pArg = pArgList->car();
        
        if (pArg->is<LispInteger>())
        {
            iResult += pArg.cast<LispInteger>()->value();
        }
        else if (pArg->is<LispNumber>())
        {
            integerOnly = false;
            nResult += pArg.cast<LispNumber>()->value();
        }
        else
        {
            throw exceptions::InvalidInput("Invalid argument type for '+'");
        }

        if(isNil(pArgList->cdr())) { break; }
        
        pArgList = pArgList->cdr().cast<LispCons>();
    }

    if (integerOnly)
    {
        return LispInteger::create(iResult);
    }

    return LispNumber::create(iResult + nResult);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::sub(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 1 argument, got none.");
    }

    auto pArgList = pArgs.cast<LispCons>();
    LispInteger::Number_t iResult = 0;
    LispNumber::Number_t nResult = 0;

    bool integerOnly = true;

    auto pArg = pArgList->car();

    if(pArg->is<LispInteger>())
    {
        iResult = pArg.cast<LispInteger>()->value();
    }
    else if(pArg->is<LispNumber>())
    {
        integerOnly = false;
        nResult = pArg.cast<LispNumber>()->value();
    }
    else
    {
        throw exceptions::InvalidInput("Invalid argument type for '-'");
    }

    if(isNil(pArgList->cdr()))
    {
        if(integerOnly)
        {
            return LispInteger::create(-iResult);
        }

        return LispNumber::create(-nResult);
    }

    pArgList = pArgList->cdr().cast<LispCons>();

    while(true)
    {
        auto pArg = pArgList->car();
        
        if (pArg->is<LispInteger>())
        {
            iResult -= pArg.cast<LispInteger>()->value();
        }
        else if (pArg->is<LispNumber>())
        {
            integerOnly = false;
            nResult -= pArg.cast<LispNumber>()->value();
        }
        else
        {
            throw exceptions::InvalidInput("Invalid argument type for 'add'");
        }

        if(isNil(pArgList->cdr())) { break; }
        
        pArgList = pArgList->cdr().cast<LispCons>();
    }

    if (integerOnly)
    {
        return LispInteger::create(iResult);
    }

    return LispNumber::create(iResult + nResult);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::mul(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 1 argument, got none.");
    }

    auto pArgList = pArgs.cast<LispCons>();
    LispInteger::Number_t iResult = 1;
    LispNumber::Number_t nResult = 1;

    bool integerOnly = true;

    while(true)
    {
        auto pArg = pArgList->car();
        
        if (pArg->is<LispInteger>())
        {
            iResult *= pArg.cast<LispInteger>()->value();
        }
        else if (pArg->is<LispNumber>())
        {
            integerOnly = false;
            nResult *= pArg.cast<LispNumber>()->value();
        }
        else
        {
            throw exceptions::InvalidInput("Invalid argument type for '*'");
        }

        if(isNil(pArgList->cdr())) { break; }
        
        pArgList = pArgList->cdr().cast<LispCons>();
    }

    if (integerOnly)
    {
        return LispInteger::create(iResult);
    }

    return LispNumber::create(iResult * nResult);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::modulo(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    EZ_LOG_BLOCK("builtin::modulo");
    ezLog::VerboseDebugMessage("Args: %s", pArgs->toString().GetData());

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(2);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 2);

    auto pLhs = pArgList->car();
    auto pRhs = pArgList->cdr().cast<LispCons>()->car();

    ezLog::VerboseDebugMessage("lhs (%s): %s, rhs (%s): %s",
                               pLhs->type().name,
                               pLhs->toString().GetData(),
                               pRhs->type().name,
                               pRhs->toString().GetData());

    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pLhs, LispInteger);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pRhs, LispInteger);

    if (pRhs.cast<LispInteger>()->value() == 0)
    {
        throw exceptions::InvalidInput("Division by 0!");
    }

    auto result = pLhs.cast<LispInteger>()->value() % pRhs.cast<LispInteger>()->value();

    return LispInteger::create(result);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::equals(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    if(isNil(pArgs))
    {
        throw exceptions::InvalidInput("Expected at least 2 arguments, got none.");
    }

    std::function<Ptr<LispBool>(Ptr<LispObject>, Ptr<LispObject>)> helper;
    helper = [&](Ptr<LispObject> pReference, Ptr<LispCons> pRestList)
    {
        if (*pReference != *pRestList->car())
        {
            return LCPP_FALSE;
        }

        if(!isNil(pRestList->cdr()))
        {
            return helper(pReference, pRestList->cdr().cast<LispCons>());
        }

        return LCPP_TRUE;
    };

    auto pArgList = pArgs.cast<LispCons>();

    if(isNil(pArgList->cdr()))
    {
        throw exceptions::InvalidInput("Expected at least 2 arguments, got 1.");
    }

    EZ_ASSERT(pArgList->cdr()->is<LispCons>(), "Invalid input.");

    return helper(pArgList->car(), pArgList->cdr().cast<LispCons>());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::objectEquals(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(2);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 2);

    if(*pArgList->car() == *pArgList->cdr().cast<LispCons>()->car())
    {
        return LCPP_TRUE;
    }

    return LCPP_FALSE;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::setRecursionLimit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispInteger);

    auto newLimit = pArgList->car().cast<LispInteger>()->value();
    LispRuntime::instance()->recursionLimit(ezUInt32(newLimit));

    return LCPP_VOID;
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::getRecursionLimit(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    auto limit = LispRuntime::instance()->recursionLimit();
    return LispInteger::create(limit);
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::cons(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(2);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 2);

    return LispRuntime::instance()->factory()->createCons(pArgList->car(), pArgList->cdr().cast<LispCons>()->car());
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::car(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispCons);

    return pArgList->car().cast<LispCons>()->car();
}

lcpp::Ptr<lcpp::LispObject>
lcpp::builtin::cdr(Ptr<Environment> pEnv, Ptr<LispObject> pArgs)
{
    LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL(1);

    auto pArgList = pArgs.cast<LispCons>();

    LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT(pArgList, 1);
    LCPP_BUILTIN_FUNCTION_CHECK_TYPE(pArgList->car(), LispCons);

    return pArgList->car().cast<LispCons>()->cdr();
}

#undef LCPP_BUILTIN_FUNCTION_CHECK_TYPE
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_COUNT
#undef LCPP_BUILTIN_FUNCTION_CHECK_ARG_NOT_NIL
