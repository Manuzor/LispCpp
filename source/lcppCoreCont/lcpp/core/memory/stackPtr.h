#pragma once

namespace lcpp
{
    class GarbageCollector;
    class CollectableBase;

    class StackPtrBase
    {
        friend GarbageCollector;
    protected:
        mutable Ptr<CollectableBase> m_ptr;
    };

    template<typename T>
    class StackPtr : public StackPtrBase
    {
        //EZ_CHECK_AT_COMPILETIME_MSG((std::is_convertible<T, CollectableBase>::value), "T needs to be a collectable object!");
    public:
        // Default construct as nullptr
        StackPtr();

        // Copy constructor
        StackPtr(const StackPtr& rhs);

        // Move constructor
        StackPtr(StackPtr&& rhs);

        // Construct from Ptr
        StackPtr(Ptr<T> ptr);

        // Construct from raw ptr
        StackPtr(T* ptr);

        // Construct from other StackPtr<>
        template<typename T_Other>
        StackPtr(const StackPtr<T_Other>& other);

        ~StackPtr();

        void operator =(const StackPtr& toCopy);
        void operator =(StackPtr&& toMove);
        void operator =(Ptr<T> ptr);
        void operator =(T* ptr);

        T* operator ->() const;
        T& operator *() const;

        T* get() const;
        bool isNull() const;

        template<typename T_Other>
        StackPtr<T_Other> cast() const;

        operator bool() const;

        operator Ptr<T>() { return m_ptr.cast<T>(); }

    private:
        void addToGc() const;
        void removeFromGc() const;
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
