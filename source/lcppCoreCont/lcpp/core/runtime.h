#pragma once

#include "lcpp/core/containers/instanceTable.h"

namespace lcpp
{
    class LispObject;

    class LCPP_API_CORE_CONT LispRuntime
    {
        friend struct RecursionCounter;
    public:

        static Ptr<LispRuntime> defaultInstance();
        static Ptr<LispRuntime> instance();
        static void patchInstance(Ptr<LispRuntime> pNewInstance);

    public:

        struct InstanceTables
        {
            // TODO Add tables.
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

        Ptr<const LispObject> syntaxEnvironment() const;
        Ptr<LispObject> syntaxEnvironment();
        void syntaxEnvironment(Ptr<LispObject> value);

        Ptr<const LispObject> globalEnvironment() const;
        Ptr<LispObject> globalEnvironment();
        void globalEnvironment(Ptr<LispObject> value);

        const InstanceTables& instanceTables() const;

        ezUInt32 recursionDepth() const;

        ezUInt32 recursionLimit() const;
        void recursionLimit(ezUInt32 newLimit);

    private:
        static Ptr<LispRuntime> s_pInstance;

        Ptr<ezAllocatorBase> m_pAllocator;

        Ptr<LispObject> m_pSyntaxEnvironment;
        Ptr<LispObject> m_pGlobalEnvironment;

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

#include "lcpp/core/impl/runtime.inl"
