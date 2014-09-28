#pragma once

#ifdef LCPP_EXPORT_FOUNDATION
#define LCPP_API_FOUNDATION __declspec(dllexport)
#else
#define LCPP_API_FOUNDATION __declspec(dllimport)
#endif // LCPP_EXPORT_FOUNDATION

// stl dependencies
#include <iostream>

// ezEngine stuff
#include "lcpp/foundation/dependencies/ezEngine.h"

#define LCPP_OVERRIDE override
#define LCPP_FINAL final

#define LCPP_Const(declaration) declaration const

#include "lcpp/foundation/memory/allocator.h"

#include "lcpp/foundation/wrapper.h"

#include "lcpp/foundation/rangeUtils.h"
#include "lcpp/foundation/ptr.h"

#define LCPP_NEW(allocator, type)                   EZ_NEW(allocator, type)
#define LCPP_NEW_ARRAY(allocator, type, count)      EZ_NEW_ARRAY(allocator, type, count)
#define LCPP_NEW_RAW_BUFFER(allocator, type, count) EZ_NEW_RAW_BUFFER(allocator, type, count)

namespace lcpp
{
    template<typename T>
    EZ_FORCE_INLINE
    void deleteHelper(Ptr<ezAllocatorBase> pAllocator, Ptr<T>& pMem)
    {
        ezInternal::Delete(pAllocator.get(), pMem.get());
        pMem = nullptr;
    }

    template<typename T>
    EZ_FORCE_INLINE
    void deleteArrayHelper(Ptr<ezAllocatorBase> pAllocator, Ptr<T>& pMem)
    {
        ezInternal::DeleteArray<T>(pAllocator.get(), pMem.get());
        pMem = nullptr;
    }

    template<typename T>
    EZ_FORCE_INLINE
    void deleteRawBufferHelper(Ptr<ezAllocatorBase> pAllocator, Ptr<T>& pMem)
    {
        ezInternal::DeleteRawBuffer<T>(pAllocator.get(), pMem.get());
        pMem = nullptr;
    }
}

#define LCPP_DELETE(pAllocator, pMem)            deleteHelper(pAllocator, pMem)
#define LCPP_DELETE_ARRAY(pAllocator, pMem)      deleteArrayHelper(pAllocator, pMem)
#define LCPP_DELETE_RAW_BUFFER(pAllocator, pMem) deleteRawBufferHelper(pAllocator, pMem)

#define LCPP_DISALLOW_COPY_ASSIGNMENT(type) private: void operator = (const type&)
#define LCPP_DISALLOW_CONSTRUCTION(type) EZ_DISALLOW_COPY_AND_ASSIGN(type); type(); ~type()
#define LCPP_UNUSED(anything) ((void)anything)
#define LCPP_NOT_IMPLEMENTED EZ_REPORT_FAILURE("Not implemented."); throw exceptions::NotImplemented()

#include "lcpp/foundation/exceptionBase.h"

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
