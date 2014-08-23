#pragma once
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    template<>
    struct Ptr<LispObject>
    {
        // Default construct as nullptr
        Ptr();

        // Copy constructor
        Ptr(const Ptr& rhs);

        // Move constructor
        Ptr(Ptr&& rhs);

        // Construct from raw pointer
        Ptr(LispObject* pPtr);

        ~Ptr();

        void operator =(const Ptr& toCopy);
        void operator =(Ptr&& toMove);
        void operator =(LispObject* pPtr);

        LispObject* operator ->() const;
        LispObject& operator *() const;

        LispObject* get() const;
        bool isNull() const;

        operator bool() const;

    private:
        LispObject* m_pPtr;
    };

    class LCPP_API_CORE_CONT CollectingAllocator :
        public ezAllocatorBase
    {
    public:

        CollectingAllocator(ezAllocatorBase* pParentAllocator);

        virtual void* Allocate(size_t uiSize, size_t uiAlign) override;

        virtual void Deallocate(void* ptr) override;

        virtual size_t AllocatedSize(const void* ptr) override;

        virtual Stats GetStats() const override;

    private:
        ezAllocatorBase* m_pParent;
    };

    LCPP_API_CORE_CONT ezAllocatorBase* getCollectingAllocator();
}

#include "lcpp/core/memory/impl/garbageCollection.inl"
