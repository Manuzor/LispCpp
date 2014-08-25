#include "lcpp/core/memory/refIndex.h"
#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr() :
        m_pGarbageCollector(),
        m_refIndex(RefIndex::invalidValue())
    {
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(const Ptr& rhs) :
        m_pGarbageCollector(rhs.m_pGarbageCollector),
        m_refIndex(rhs.m_refIndex)
    {
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(Ptr&& rhs) :
        m_pGarbageCollector(rhs.m_pGarbageCollector),
        m_refIndex(rhs.m_refIndex)
    {
        rhs.m_pGarbageCollector = nullptr;
        rhs.m_refIndex = RefIndex::invalidValue();
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::Ptr(LispObject* pPtr) :
        m_pGarbageCollector(),
        m_refIndex(RefIndex::invalidValue())
    {
        if (pPtr)
        {
            m_pGarbageCollector = pPtr->getGarbageCollector();
            m_refIndex = pPtr->getRefIndex();
        }
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::~Ptr()
    {
    }

    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(const Ptr& toCopy)
    {
        Ptr<LispObject> copy(toCopy);

        ezMath::Swap(m_pGarbageCollector, copy.m_pGarbageCollector);
        ezMath::Swap(m_refIndex, copy.m_refIndex);
    }
    
    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(Ptr&& toMove)
    {
        m_pGarbageCollector = toMove.m_pGarbageCollector;
        m_refIndex = toMove.m_refIndex;

        toMove.m_pGarbageCollector = nullptr;
        toMove.m_refIndex = RefIndex::invalidValue();
    }
    
    EZ_FORCE_INLINE
    void Ptr<LispObject>::operator =(LispObject* pPtr)
    {
        if (pPtr)
        {
            m_pGarbageCollector = pPtr->getGarbageCollector();
            m_refIndex = pPtr->getRefIndex();
        }
    }
    
    EZ_FORCE_INLINE
    LispObject* Ptr<LispObject>::get() const
    {
        if (isNull())
        {
            return nullptr;
        }

        EZ_ASSERT(m_pGarbageCollector, "");
        EZ_ASSERT(m_refIndex.isValid(), "");

        return m_pGarbageCollector->getPointer<LispObject>(m_refIndex);
    }
    
    EZ_FORCE_INLINE
    bool Ptr<LispObject>::isNull() const
    {
        return m_pGarbageCollector.isNull() || !m_refIndex.isValid();
    }
    
    EZ_FORCE_INLINE
    Ptr<LispObject>::operator bool() const
    {
        return !isNull();
    }
    
    EZ_FORCE_INLINE
    LispObject* Ptr<LispObject>::operator ->() const
    {
        return get();
    }
    
    EZ_FORCE_INLINE
    LispObject& Ptr<LispObject>::operator *() const
    {
        return *get();
    }
}
