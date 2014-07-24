#pragma once

#include "lcpp/core/instancing.h"

namespace lcpp
{
    class Reader;
    class IEvaluator;
    class TypeFactory;
    class LispEnvironment;

    class LCPP_API_CORE LispRuntime
    {
        friend struct RecursionCounter;
    public:

        static Ptr<LispRuntime> defaultInstance();
        static Ptr<LispRuntime> instance();
        static void patchInstance(Ptr<LispRuntime> pNewInstance);

    public:

        struct InstanceTables
        {
            Ptr<InstanceTable_Symbols> pSymbols;
            Ptr<InstanceTable_Integers> pIntegers;
        };

    public:

        LispRuntime();

        // Initialization
        //////////////////////////////////////////////////////////////////////////
        void initialize();
        void shutdown();

        //////////////////////////////////////////////////////////////////////////

        Ptr<ezAllocatorBase> allocator();
        void allocator(Ptr<ezAllocatorBase> value);

        Ptr<const Reader> reader() const;
        Ptr<Reader> reader();
        void reader(Ptr<Reader> value);

        Ptr<const IEvaluator> evaluator() const;
        Ptr<IEvaluator> evaluator();
        void evaluator(Ptr<IEvaluator> value);

        Ptr<const LispEnvironment> syntaxEnvironment() const;
        Ptr<LispEnvironment> syntaxEnvironment();
        void syntaxEnvironment(Ptr<LispEnvironment> value);

        Ptr<const LispEnvironment> globalEnvironment() const;
        Ptr<LispEnvironment> globalEnvironment();
        void globalEnvironment(Ptr<LispEnvironment> value);

        const InstanceTables& instanceTables() const;

        ezUInt32 recursionDepth() const;

        ezUInt32 recursionLimit() const;
        void recursionLimit(ezUInt32 newLimit);

    private:
        static Ptr<LispRuntime> s_pInstance;

        Ptr<ezAllocatorBase> m_pAllocator;

        Ptr<Reader> m_pReader;
        Ptr<IEvaluator> m_pEvaluator;
        Ptr<LispEnvironment> m_pSyntaxEnvironment;
        Ptr<LispEnvironment> m_pGlobalEnvironment;

        InstanceTables m_instanceTables;

        ezUInt32 m_recursionLimit;
        ezUInt32 m_recursionDepth;

        // Called in initialize()
        void registerBuiltIns();

        // Used by the RecursionCounter
        void increaseRecursionDepth();

        // Used by the RecursionCounter
        void decreaseRecursionDepth();
    };
}

#include "lcpp/core/implementation/runtime.inl"
