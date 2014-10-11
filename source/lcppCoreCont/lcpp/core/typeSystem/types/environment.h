#pragma once
#include "lcpp/core/typeSystem/types/environmentCommon.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace env
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParent, Ptr<LispObject>pName);

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispObject> pName);

        LCPP_API_CORE_CONT Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent);

        LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pEnv);
        LCPP_API_CORE_CONT Ptr<LispObject> getQualifiedName(Ptr<LispObject> pEnv);
        LCPP_API_CORE_CONT Ptr<LispObject> getParent(Ptr<LispObject> pEnv);

        LCPP_API_CORE_CONT void addBinding(Ptr<LispObject> pEnv,
                                           Ptr<LispObject> pSymbol,
                                           Ptr<LispObject> pValue);

        LCPP_API_CORE_CONT ezResult setBinding(Ptr<LispObject> pEnv,
                                               Ptr<LispObject> pSymbol,
                                               Ptr<LispObject> pValue);

        LCPP_API_CORE_CONT ezResult getBinding(Ptr<LispObject> pEnv,
                                               Ptr<LispObject> pSymbol,
                                               Ptr<LispObject>& out_pValue);

        LCPP_API_CORE_CONT BindingLocation existsBinding(Ptr<LispObject> pEnv,
                                                         Ptr<LispObject> pSymbol);

        Ptr<LispObject> toString(Ptr<LispObject> pObject);

        namespace detail
        {
            LCPP_API_CORE_CONT HashTable& getTable(Ptr<LispObject> pEnv);
        }
    };
}

#include "lcpp/core/typeSystem/types/impl/environment.inl"
