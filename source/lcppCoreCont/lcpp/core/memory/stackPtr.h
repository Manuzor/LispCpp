#pragma once

namespace lcpp
{
    class GarbageCollector;
    class CollectableBase;

    class StackPtrBase
    {
        friend GarbageCollector;
    public:

        enum { NumMaxStackPtrs = 1024 };
        LCPP_API_CORE_CONT static CollectableBase* volatile s_ptrTable[NumMaxStackPtrs];
        LCPP_API_CORE_CONT static ezUInt32 s_uiNextIndex;

    protected:

        StackPtrBase(CollectableBase* ptr);
        StackPtrBase(const StackPtrBase& toCopy);

        ~StackPtrBase();

        void operator=(const StackPtrBase& toCopy);
        void operator=(CollectableBase* ptr);

        CollectableBase* get();
        bool isNull();

    protected:
        ezUInt32 m_uiIndex;
        LCPP_InDebug( mutable CollectableBase* m_pLastLookup; );
    };

    template<typename T>
    class StackPtr : public StackPtrBase
    {
        //EZ_CHECK_AT_COMPILETIME_MSG((std::is_convertible<T, CollectableBase>::value), "T needs to be a collectable object!");
    public:

        // Disable move constructor and assignment
        StackPtr(StackPtr&& toMove) = delete;
        void operator =(StackPtr&& toMove) = delete;

        // Copy constructor
        StackPtr(StackPtr& rhs);

        // Construct from Ptr
        StackPtr(Ptr<T> ptr);

        // Construct from raw ptr
        StackPtr(T* ptr);

        void operator =(StackPtr& toCopy);
        void operator =(Ptr<T> ptr);
        void operator =(T* ptr);

        T* operator ->();
        T& operator *();

        T* get();
        bool isNull();

        template<typename T_Other>
        StackPtr<T_Other> cast();

        operator bool();

        operator Ptr<T>() { return get(); }
        operator T*() { return get(); }
    };
}

/// Template specialization so Ptr<...> can be used in ezHashTable.
template<typename T>
struct ezHashHelper<lcpp::StackPtr<T>>
{
    static ezUInt32 Hash(lcpp::StackPtr<T> value);
    static bool Equal(lcpp::StackPtr<T> a, lcpp::StackPtr<T> b);
};

#include "lcpp/core/memory/impl/stackPtr.inl"
