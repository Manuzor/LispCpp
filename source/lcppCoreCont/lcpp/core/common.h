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

#include "lcpp/core/memory/ptrSpecialization.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/memory/impl/ptrSpecialization.inl"

#include "lcpp/core/commonTypes.h"

#define LCPP_DeclareRawDataMember(typeName, memberName) char memberName [sizeof(typeName)]
