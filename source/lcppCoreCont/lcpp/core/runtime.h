#pragma once

#include "lcpp/core/readerState.h"
#include "lcpp/core/printerState.h"

namespace lcpp
{
    class Stack;
    class LispObject;

    class LCPP_API_CORE_CONT LispRuntimeState
    {
        friend class RecursionCounter;
    public:

        class Statistics
        {
        public:
            ezUInt32 m_initializationCount;
            ezUInt32 m_shutdownCount;
        };

    public:

        LispRuntimeState();

        // Initialization
        //////////////////////////////////////////////////////////////////////////
        void initialize(ezAllocatorBase* = nullptr);
        void shutdown();

        //////////////////////////////////////////////////////////////////////////

        Statistics m_stats;

        Ptr<GarbageCollector> getGarabgeCollector();

        Ptr<LispObject> getSyntaxEnvironment();

        Ptr<LispObject> getGlobalEnvironment();

        Ptr<reader::State> getReaderState();
        Ptr<const reader::State> getReaderState() const;

        Ptr<printer::State> getPrinterState();
        Ptr<const printer::State> getPrinterState() const;

        void setBaseDirectory(const char* szName);
        const char* getBaseDirectory() const;

        void setUserDirectory(const char* szName);
        const char* getUserDirectory() const;

        bool getBreakExecution() const { return m_bBreakExecution; }
        void setBreakExecution(bool value) { m_bBreakExecution = value; }

    private:

        ezAllocatorBase* m_pAllocator;
        Ptr<GarbageCollector> m_pGC;

        LispObject* m_pSyntaxEnvironment;
        LispObject* m_pGlobalEnvironment;

        reader::State* m_pReaderState;
        printer::State* m_pPrinterState;

        ezString m_baseDirectory;
        ezString m_userDirectory;

        bool m_bBreakExecution;

    private:

        // Called in initialize()
        void registerBuiltIns();
    };
}

#include "lcpp/core/impl/runtime.inl"
