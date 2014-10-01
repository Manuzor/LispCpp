#pragma once
#include "lcpp/core/typeSystem/types/environmentCommon.h"

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace env
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT StackPtr<LispObject> create(StackPtr<LispObject> pParent, StackPtr<LispObject>pName);

        LCPP_API_CORE_CONT StackPtr<LispObject> createTopLevel(StackPtr<LispObject> pName);

        LCPP_API_CORE_CONT StackPtr<LispObject> createAnonymous(StackPtr<LispObject> pParent);

        LCPP_API_CORE_CONT StackPtr<LispObject> getName(StackPtr<LispObject> pEnv);
        LCPP_API_CORE_CONT StackPtr<LispObject> getQualifiedName(StackPtr<LispObject> pEnv);
        LCPP_API_CORE_CONT StackPtr<LispObject> getParent(Ptr<LispObject> pEnv);

        LCPP_API_CORE_CONT void addBinding(StackPtr<LispObject> pEnv,
                                           StackPtr<LispObject> pSymbol,
                                           StackPtr<LispObject> pValue);

        LCPP_API_CORE_CONT ezResult setBinding(StackPtr<LispObject> pEnv,
                                               StackPtr<LispObject> pSymbol,
                                               StackPtr<LispObject> pValue);

        LCPP_API_CORE_CONT ezResult getBinding(Ptr<LispObject> pEnv,
                                               Ptr<LispObject> pSymbol,
                                               Ptr<LispObject>& out_pValue);

        LCPP_API_CORE_CONT BindingLocation existsBinding(StackPtr<LispObject> pEnv,
                                                         StackPtr<LispObject> pSymbol);

        StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

        namespace detail
        {
            LCPP_API_CORE_CONT HashTable& getTable(Ptr<LispObject> pEnv);
        }
    };
}

#include "lcpp/core/typeSystem/types/impl/environment.inl"
