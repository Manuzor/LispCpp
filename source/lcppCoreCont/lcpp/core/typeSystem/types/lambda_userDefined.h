#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace userDefined
        {

            typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

            LCPP_API_CORE_CONT const MetaInfo& metaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Ptr<LispObject> pArgList, Ptr<LispObject> pBodyList);

            //////////////////////////////////////////////////////////////////////////

            class Data
            {
            public:

                Ptr<LispObject> getName();
                void setName(Ptr<LispObject> pNewName);

                Ptr<LispObject> getEnv();
                Ptr<LispObject> getArgList();
                Ptr<LispObject> getBodyList();

            public:

                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pEnv);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pArgList);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pBodyList);

            };

            //////////////////////////////////////////////////////////////////////////

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getArgList(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getBodyList(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

            namespace detail
            {
                Ptr<LispObject> call_updateEnv(Ptr<LispObject> pCont);
                Ptr<LispObject> call_evalBody(Ptr<LispObject> pCont);
                Ptr<LispObject> call_evalBody_2(Ptr<LispObject> pCont);
            }
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambda_userDefined.inl"
