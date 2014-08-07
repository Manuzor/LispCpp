#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Stack;

    namespace cons
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pCar, Ptr<LispObject> pCdr);

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            Ptr<LispObject> getCar();
            Ptr<LispObject> getCdr();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pCar);
            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pCdr);
        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT Ptr<LispObject> getCar(Ptr<LispObject> pCons);

        LCPP_API_CORE_CONT Ptr<LispObject> getCdr(Ptr<LispObject> pCons);

        LCPP_API_CORE_CONT ezUInt32 pushAll(Ptr<LispObject> pCons, Ptr<Stack> pStack);
        LCPP_API_CORE_CONT ezUInt32 pushAllReverse(Ptr<LispObject> pCons, Ptr<Stack> pStack);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);
    }

    LCPP_API_CORE_CONT bool isCons(Ptr<LispObject> pObject);
}

#include "lcpp/core/typeSystem/types/impl/cons.inl"
