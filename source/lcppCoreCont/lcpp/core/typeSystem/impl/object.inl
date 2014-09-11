#include "lcpp/core/memory/garbageCollection.h"

namespace lcpp
{
    namespace object
    {
        template<typename T_Data>
        inline
        Ptr<LispObject>
        create(Ptr<const MetaInfo> pMetaInfo)
        {
            // Helper struct to determine the minimum memory needed for this lisp object using T_Data
            struct LispObjectProxy : public LispObject
            {
                T_Data m_data;
            };

            struct LispObjectProxyChecker
            {
                LispObject o;
                T_Data d;
            };

            EZ_CHECK_AT_COMPILETIME(sizeof(LispObjectProxy) == sizeof(LispObjectProxyChecker));

            //////////////////////////////////////////////////////////////////////////

            // TODO The runtime state should be passed to this function, which should contain the garbage collector.
            auto pGarbageCollector = getGarbageCollector();

            auto pInstance = pGarbageCollector->create<LispObjectProxy>(pMetaInfo);

            return pInstance.cast<LispObject>();
        }
    }
}
