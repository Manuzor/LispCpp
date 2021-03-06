#pragma once

#ifdef LCPP_EXPORT_CORE_CONT
#define LCPP_API_CORE_CONT __declspec(dllexport)
#else
#define LCPP_API_CORE_CONT __declspec(dllimport)
#endif // LCPP_EXPORT_FOUNDATION

#include "lcpp/core/dependencies/ezEngine.h"

#if EZ_ENABLED(LCPP_LOGGING)

#define LCPP_LogBlock EZ_LOG_BLOCK
#define LCPP_LogError ezLog::Error
#define LCPP_LogSeriousWarning ezLog::SeriousWarning
#define LCPP_LogWarning ezLog::Warning
#define LCPP_LogSuccess ezLog::Success
#define LCPP_LogInfo ezLog::Info
#define LCPP_LogDev ezLog::Dev
#define LCPP_LogDebug ezLog::Debug

#if EZ_ENABLED(LCPP_VERBOSE_LOGGING)
// Enable this to allow verbose debug messages
#define LCPP_LogVerboseDebugMessage ezLog::Debug
#else
#define LCPP_LogVerboseDebugMessage(...)
#endif

#else

#if EZ_ENABLED(LCPP_VERBOSE_LOGGING)
#warning "There is no point in enabling verbose logging without enabling regular logging."
#endif

#define LCPP_LogBlock(...)
#define LCPP_LogError(...)
#define LCPP_LogSeriousWarning(...)
#define LCPP_LogWarning(...)
#define LCPP_LogSuccess(...)
#define LCPP_LogInfo(...)
#define LCPP_LogDev(...)
#define LCPP_LogDebug(...)
#define LCPP_LogVerboseDebugMessage(...)

#endif

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)
#define LCPP_InDebug(...) __VA_ARGS__
#define LCPP_InNonDebug(...)
#else
#define LCPP_InDebug(...)
#define LCPP_InNonDebug(...) __VA_ARGS__
#endif

#include "lcpp/core/memory/garbageCollection.h"

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)
extern lcpp::GarbageCollector* g_pGC;
#endif

#include "lcpp/core/commonTypes.h"

#include "lcpp/core/typeSystem/objectData.h"

#define LCPP_DeclareRawDataMember(typeName, memberName) char memberName [sizeof(typeName)]
