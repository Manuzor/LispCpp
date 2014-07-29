#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace env
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pName, Ptr<LispObject> pParent);

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispObject> pName);

        LCPP_API_CORE_CONT Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent);

        //////////////////////////////////////////////////////////////////////////
        
        class Data
        {
        public:

            Ptr<LispObject> getName();
            Ptr<LispObject> getParent();
            HashTable& getTable();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pParent);

            LCPP_DeclareRawDataMember(HashTable, m_table);
        };

        //////////////////////////////////////////////////////////////////////////

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

        namespace detail
        {
            LCPP_API_CORE_CONT HashTable& getTable(Ptr<LispObject> pEnv);
        }
    };
}

#include "lcpp/core/typeSystem/types/impl/environment.inl"
