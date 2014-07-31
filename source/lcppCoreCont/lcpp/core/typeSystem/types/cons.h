#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

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

    }

}

#include "lcpp/core/typeSystem/types/impl/cons.inl"
