#pragma once

namespace lcpp
{
    class Reader;
    class IEvaluator;
    class TypeFactory;
    class Environment;

    class LCPP_CORE_API SchemeRuntime
    {
    public:
        struct RecursionCounter
        {
            ezUInt32& value;
            RecursionCounter(ezUInt32& value) :
                value(value)
            {
                ++value;
            }

            ~RecursionCounter()
            {
                --value;
            }
        };
    public:
        SchemeRuntime();

        void initialize();
        void shutdown();

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
        RecursionCounter createRecursionCounter();

        ezUInt32 recursionLimit() const;
        void recursionLimit(ezUInt32 newLimit);

    private:
        Ptr<ezAllocatorBase> m_pAllocator;
        Ptr<TypeFactory> m_pFactory;

        Ptr<Reader> m_pReader;
        Ptr<IEvaluator> m_pEvaluator;
        Ptr<Environment> m_pSyntaxEnvironment;
        Ptr<Environment> m_pGlobalEnvironment;

        ezUInt32 m_recursionLimit;
        ezUInt32 m_recursionDepth;
    };
}

#include "lcpp/core/implementation/runtime_inl.h"
