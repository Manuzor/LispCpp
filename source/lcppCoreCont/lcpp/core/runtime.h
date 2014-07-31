#pragma once

#include "lcpp/core/reader.h"

namespace lcpp
{
    class Stack;
    class LispObject;

    class LCPP_API_CORE_CONT LispRuntime
    {
        friend struct RecursionCounter;
    public:

        static Ptr<LispRuntime> defaultInstance();
        static Ptr<LispRuntime> instance();
        static void patchInstance(Ptr<LispRuntime> pNewInstance);

    public:

        struct Statistics
        {
            ezUInt32 m_initializationCount;
            ezUInt32 m_shutdownCount;
        };

    public:

        LispRuntime();

        // Initialization
        //////////////////////////////////////////////////////////////////////////
        void initialize();
        void shutdown();

        //////////////////////////////////////////////////////////////////////////

        Statistics m_stats;

        Ptr<ezAllocatorBase> allocator();

        Ptr<const LispObject> getSyntaxEnvironment() const;
        Ptr<LispObject> getSyntaxEnvironment();

        Ptr<const LispObject> getGlobalEnvironment() const;
        Ptr<LispObject> getGlobalEnvironment();

        Ptr<reader::State> getReaderState();
        Ptr<const reader::State> getReaderState() const;

        ezUInt32 getRecursionDepth() const;

        ezUInt32 getRecursionLimit() const;
        void setRecursionLimit(ezUInt32 newLimit);

    private:

        static Ptr<LispRuntime> s_pInstance;

        Ptr<ezAllocatorBase> m_pAllocator;

        Ptr<LispObject> m_pSyntaxEnvironment;
        Ptr<LispObject> m_pGlobalEnvironment;

        Ptr<reader::State> m_pReaderState;

        ezUInt32 m_recursionLimit;
        ezUInt32 m_recursionDepth;

    private:

        // Called in initialize()
        void registerBuiltIns();

        // Used by the RecursionCounter
        void increaseRecursionDepth();

        // Used by the RecursionCounter
        void decreaseRecursionDepth();
    };
}

#define LCPP_pRuntime ::lcpp::LispRuntime::instance()

#include "lcpp/core/impl/runtime.inl"
