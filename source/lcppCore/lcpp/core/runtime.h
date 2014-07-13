#pragma once

namespace lcpp
{
    class Reader;
    class IEvaluator;
    class TypeFactory;
    class Environment;

    class LCPP_CORE_API LispRuntime
    {
        friend struct RecursionCounter;
    public:

        static Ptr<LispRuntime> defaultInstance();
        static Ptr<LispRuntime> instance();
        static void patchInstance(Ptr<LispRuntime> pNewInstance);

    public:

        LispRuntime();

        // Initialization
        //////////////////////////////////////////////////////////////////////////
        void initialize();
        void shutdown();

        //////////////////////////////////////////////////////////////////////////

        Ptr<ezAllocatorBase> allocator();
        void allocator(Ptr<ezAllocatorBase> value);

        Ptr<const TypeFactory> factory() const;
        Ptr<TypeFactory> factory();
        void factory(Ptr<TypeFactory> value);

        Ptr<const Reader> reader() const;
        Ptr<Reader> reader();
        void reader(Ptr<Reader> value);

        Ptr<const IEvaluator> evaluator() const;
        Ptr<IEvaluator> evaluator();
        void evaluator(Ptr<IEvaluator> value);

        Ptr<const Environment> syntaxEnvironment() const;
        Ptr<Environment> syntaxEnvironment();
        void syntaxEnvironment(Ptr<Environment> value);

        Ptr<const Environment> globalEnvironment() const;
        Ptr<Environment> globalEnvironment();
        void globalEnvironment(Ptr<Environment> value);

        ezUInt32 recursionDepth() const;

        ezUInt32 recursionLimit() const;
        void recursionLimit(ezUInt32 newLimit);

    private:
        static Ptr<LispRuntime> s_pInstance;

        Ptr<ezAllocatorBase> m_pAllocator;
        Ptr<TypeFactory> m_pFactory;

        Ptr<Reader> m_pReader;
        Ptr<IEvaluator> m_pEvaluator;
        Ptr<Environment> m_pSyntaxEnvironment;
        Ptr<Environment> m_pGlobalEnvironment;

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

#include "lcpp/core/implementation/runtime_inl.h"
