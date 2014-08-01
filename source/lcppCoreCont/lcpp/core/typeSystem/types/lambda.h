#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Function_t pFunction);

        //////////////////////////////////////////////////////////////////////////

        class Data
        {
        public:

            Ptr<LispObject> getEnv();
            Function_t getFunction();

        public:

            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pEnv);
            Function_t m_pFunction;

        };

        //////////////////////////////////////////////////////////////////////////

        LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pLambda);

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> userDefinedFunction(Ptr<LispObject> pCont);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambda.inl"
