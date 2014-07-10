#pragma once

#ifdef LCPP_CORE_EXPORT
#define LCPP_CORE_API __declspec(dllexport)
#else
#define LCPP_CORE_API __declspec(dllimport)
#endif // LCPP_CORE_EXPORT

// stl dependencies
#include <iostream>

// ezEngine stuff
#include <Foundation/Basics/IncludeAll.cpp>

#define LCPP_OVERRIDE EZ_OVERRIDE
#define LCPP_FINAL final

#include "lcpp/foundation/memory/allocator.h"
#include "lcpp/foundation/rangeUtils.h"
#include "lcpp/foundation/ptr.h"

#define LCPP_NEW(allocator, type)                   EZ_NEW(allocator, type)
#define LCPP_NEW_ARRAY(allocator, type)             EZ_NEW_ARRAY(allocator, type)
#define LCPP_NEW_RAW_BUFFER(allocator, type, count) EZ_NEW_RAW_BUFFER(allocator, type, count)

#define LCPP_DELETE(allocator, ptr)            ezInternal::Delete(allocator, ptr)
#define LCPP_DELETE_ARRAY(allocator, ptr)      ezInternal::DeleteArray(allocator, ptr)
#define LCPP_DELETE_RAW_BUFFER(allocator, ptr) ezInternal::DeleteRawBuffer(allocator, ptr)

#include "lcpp/exceptions/exceptions.h"

#define LCPP_DISALLOW_COPY_ASSIGNMENT(type) private: void operator = (const type&)
#define LCPP_DISALLOW_CONSTRUCTION(type) EZ_DISALLOW_COPY_AND_ASSIGN(type); type(); ~type()
#define LCPP_UNUSED(anything) ((void)anything)

// Use VerboseDebugMessage to disable verbose debug loggings,
// otherwise use something like Info or Debug.
#define LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME Info

namespace lcpp
{
    template<typename T>
    struct ScopeExit_tpl
    {
        T m_onExitFunction;

        ScopeExit_tpl() : m_onExitFunction(nullptr) {}
        ~ScopeExit_tpl() { m_onExitFunction(); }
    };

    typedef ScopeExit_tpl<::std::function<void()>> ScopeExit;
}

// Executes code when the current scope exits. Use within a scope only!
// Example:
// LCPP_SCOPE_EXIT
// {
//     printf("You will always see me when the current scope exits!\n");
// }; // <- mind the semicolon!
#define LCPP_SCOPE_EXIT ::lcpp::ScopeExit EZ_CONCAT(scopeExit_, EZ_SOURCE_LINE) ; EZ_CONCAT(scopeExit_, EZ_SOURCE_LINE).m_onExitFunction = [&]()
