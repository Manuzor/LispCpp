#pragma once

#ifdef LCPP_EXPORT_CORE_CONT
#define LCPP_API_CORE_CONT __declspec(dllexport)
#else
#define LCPP_API_CORE_CONT __declspec(dllimport)
#endif // LCPP_EXPORT_FOUNDATION

#include "lcpp/core/dependencies/ezEngine.h"

#include "lcpp/core/memory/ptrSpecialization.h"
#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/memory/impl/ptrSpecialization.inl"

#include "lcpp/core/commonTypes.h"

#define LCPP_DeclareRawDataMember(typeName, memberName) char memberName [sizeof(typeName)]
