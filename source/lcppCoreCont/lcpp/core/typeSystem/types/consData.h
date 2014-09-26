#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Stack;

    namespace cons
    {
        class Data
        {
        public:

            Ptr<LispObject>& getCar();
            Ptr<LispObject>& getCdr();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pCar);
            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pCdr);
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/consData.inl"
