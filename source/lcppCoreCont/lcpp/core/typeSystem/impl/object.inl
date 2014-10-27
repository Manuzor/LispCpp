#include "lcpp/core/memory/garbageCollection.h"
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    namespace object
    {
        template<typename T_Data>
        EZ_FORCE_INLINE
        Ptr<LispObject> create(Ptr<const MetaInfo> pMetaInfo)
        {
            return create<T_Data>(getGarbageCollector(), pMetaInfo);
        }

        template<typename T_Data>
        EZ_FORCE_INLINE
        Ptr<LispObject> create(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo)
        {
            // Helper struct to determine the minimum memory needed for this lisp object using T_Data
            struct LispObjectProxy : public LispObject
            {
                T_Data m_userData;
            };

            struct LispObjectProxyChecker
            {
                LispObject o;
                T_Data d;
            };

            EZ_CHECK_AT_COMPILETIME(sizeof(LispObjectProxy) == sizeof(LispObjectProxyChecker));

            //////////////////////////////////////////////////////////////////////////

            auto pInstance = pGarbageCollector->create<LispObjectProxy>(pMetaInfo);
            EZ_ASSERT(pInstance->m_uiMemorySize == sizeof(LispObjectProxy), "");

            return pInstance.cast<LispObject>();
        }

        template<typename T_Data>
        EZ_FORCE_INLINE
        Ptr<LispObject> createStatic(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo)
        {
            // Helper struct to determine the minimum memory needed for this lisp object using T_Data
            struct LispObjectProxy : public LispObject
            {
                T_Data m_userData;
            };

            auto pInstance = pGarbageCollector->createStatic<LispObjectProxy>(pMetaInfo);

            return static_cast<LispObject*>(pInstance);
        }

    }
}
