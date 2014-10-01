#pragma once

namespace lcpp
{
    class LispObject;

    namespace env
    {
        class Data
        {
        public:

            Ptr<LispObject>& getName();
            Ptr<LispObject>& getParent();
            HashTable& getTable();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pParent);

            HashTable m_table;
        };
    };
}

#include "lcpp/core/typeSystem/types/impl/environmentData.inl"
