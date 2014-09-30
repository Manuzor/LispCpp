#include "lcpp/core/typeSystem/metaInfo.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    CollectableBase::CollectableBase() :
        m_bIsForwarded(false),
        m_uiMemorySize(0),
        m_uiGeneration(0)
    {
    }

    EZ_FORCE_INLINE
    void CollectableBase::setGarbageCollector(Ptr<GarbageCollector> pGarbageCollector)
    {
        m_pGarbageCollector = pGarbageCollector.get();
    }

    EZ_FORCE_INLINE
    Ptr<GarbageCollector> CollectableBase::getGarbageCollector()
    {
        return m_pGarbageCollector;
    }

    EZ_FORCE_INLINE
    Ptr<const GarbageCollector> CollectableBase::getGarbageCollector() const
    {
        return m_pGarbageCollector;
    }

    EZ_FORCE_INLINE
    void CollectableBase::setMetaInfo(Ptr<const MetaInfo> pMetaInfo)
    {
        m_pMetaInfo = pMetaInfo.get();
    }

    EZ_FORCE_INLINE
    Ptr<const MetaInfo> CollectableBase::getMetaInfo() const
    {
        return m_pMetaInfo;
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr()
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(const StackPtr& rhs)
    {
        m_ptr = rhs.m_ptr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(StackPtr&& rhs)
    {
        m_ptr = std::move(rhs.m_ptr);
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(Ptr<T> ptr)
    {
        m_ptr = ptr;
        addToGc();
    }

    template<typename T>
    template<typename T_Other>
    EZ_FORCE_INLINE
    StackPtr<T>::StackPtr(const StackPtr<T_Other>& other)
    {
        m_ptr = other.m_ptr;
        addToGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::~StackPtr()
    {
        removeFromGc();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(const StackPtr& toCopy)
    {
        StackPtr<T> copy(toCopy);

        using namespace std;
        swap(m_ptr, copy.m_ptr);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(StackPtr&& toMove)
    {
        m_ptr = std::move(toMove.m_ptr);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::operator=(Ptr<T> pPtr)
    {
        *this = StackPtr<T>(pPtr);
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::operator ->() const
    {
        return get();
    }

    template<typename T>
    EZ_FORCE_INLINE
    T& StackPtr<T>::operator *() const
    {
        return *get();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    T* StackPtr<T>::get() const
    {
        return static_cast<T*>(m_ptr.get());
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    bool StackPtr<T>::isNull() const
    {
        return m_ptr.isNull();
    }

    template<typename T>
    template<typename T_Other>
    EZ_FORCE_INLINE
    StackPtr<T_Other> StackPtr<T>::cast() const
    {
        return m_ptr.cast<T_Other>();
    }
    
    template<typename T>
    EZ_FORCE_INLINE
    StackPtr<T>::operator bool() const
    {
        return !isNull();
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::addToGc() const
    {
        if(!isNull())
            m_ptr->getGarbageCollector()->addStackPtr(this);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void StackPtr<T>::removeFromGc() const
    {
        if(!isNull())
            m_ptr->getGarbageCollector()->removeStackPtr(this);
    }

    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE
    GarbageCollector::CInfo::CInfo() :
        m_uiInitialMemoryLimit(0)
    {
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::createStatic(Ptr<const MetaInfo> pMetaInfo)
    {
        LCPP_LogBlock("GarbageCollector::createStatic");

        auto pInstance = EZ_NEW(m_pAllocator, T);
        pInstance->setGarbageCollector(this);
        pInstance->setMetaInfo(pMetaInfo);

        return pInstance;
    }

    template<typename T>
    EZ_FORCE_INLINE
    Ptr<T> GarbageCollector::create(Ptr<const MetaInfo> pMetaInfo)
    {
        LCPP_LogBlock("GarbageCollector::create");
        
        EZ_CHECK_AT_COMPILETIME((std::is_convertible<T, CollectableBase>::value));

        LCPP_LogBlock("GarbageCollector::basicCreate");
        
        T* pInstance = nullptr;

        ezUInt32 uiNumTries(0);
        while(true)
        {
            ++uiNumTries;
            auto result = m_edenSpace.allocate(pInstance);

            if (result.succeeded())
            {
                break;
            }

            if (result.isOutOfMemory())
            {
                if (uiNumTries >= 2)
                {
                    // TODO Resize the internal array?
                    EZ_REPORT_FAILURE("Out of memory!");
                    return nullptr;
                }

                collect();
            }
            else
            {
                // TODO Figure out what to do here.
                LCPP_NOT_IMPLEMENTED;
            }
        }

        new (pInstance) T();
        pInstance->m_uiMemorySize = sizeof(T);
        pInstance->m_pGarbageCollector = this;
        pInstance->m_pMetaInfo = pMetaInfo.get();
        pInstance->m_uiGeneration = m_uiCurrentGeneration;

        return pInstance;
    }

    EZ_FORCE_INLINE
    void GarbageCollector::addRoot(Ptr<CollectableBase> pCollectable)
    {
        EZ_ASSERT(!isRoot(pCollectable), "Cannot add same pointer as root more than once!");
        m_roots.PushBack(pCollectable.get());
    }

    EZ_FORCE_INLINE
    void GarbageCollector::removeRoot(Ptr<CollectableBase> pCollectable)
    {
        auto wasRemoved = m_roots.RemoveSwap(pCollectable.get());
        EZ_ASSERT(wasRemoved, "Given pointer is not a root pointer!");
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isRoot(Ptr<CollectableBase> pCollectable) const
    {
        return m_roots.Contains(pCollectable.get());
    }
    
    EZ_FORCE_INLINE
    bool GarbageCollector::isAlive(Ptr<CollectableBase> pCollectable) const
    {
        EZ_ASSERT(pCollectable->m_uiGeneration <= m_uiCurrentGeneration, "Invalid generation for object.");

        if (m_bIsCollecting)
        {
            return pCollectable->m_uiGeneration == m_uiCurrentGeneration
                || pCollectable->m_uiGeneration == m_uiCurrentGeneration - 1;
        }

        return pCollectable->m_uiGeneration == m_uiCurrentGeneration;
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isValidEdenPtr(Ptr<CollectableBase> pObject) const
    {
        auto ptr = (byte_t*)pObject.get();
        auto memory = m_edenSpace.getMemory();
        return ptr >= memory.getData() && ptr < (memory.getData() + memory.getSize());
    }
    
    EZ_FORCE_INLINE
    void GarbageCollector::addStackPtr(const StackPtrBase* stackPtr) const
    {
        m_stackReferences.PushBack(stackPtr);
    }

    EZ_FORCE_INLINE
    void GarbageCollector::removeStackPtr(const StackPtrBase* stackPtr) const
    {
        auto uiIndex = m_stackReferences.LastIndexOf(stackPtr);
        EZ_ASSERT(uiIndex != ezInvalidIndex, "");
        m_stackReferences.RemoveAtSwap(uiIndex);
    }

    EZ_FORCE_INLINE
    bool GarbageCollector::isOnStack(Ptr<CollectableBase> pCollectable) const
    {
        for (auto pStackPtr : m_stackReferences)
        {
            if (pStackPtr->m_ptr == pCollectable)
                return true;
        }

        return false;
    }

    EZ_FORCE_INLINE
    void detail::assertObjectIsAlive(Ptr<CollectableBase> pCollectable)
    {
        assertObjectIsAlive(pCollectable.get());
    }

    EZ_FORCE_INLINE
    void detail::assertObjectIsAlive(CollectableBase* pCollectable)
    {
        EZ_ASSERT(pCollectable, "Invalid pointer.");

        auto pGc = pCollectable->getGarbageCollector().get();
        EZ_ASSERT(pGc, "The object was not created with a (properly implemented) garbage collector.");

        EZ_ASSERT(pGc->isAlive(pCollectable), "The object is not alive at this point!");
    }
}
