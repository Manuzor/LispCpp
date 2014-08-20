#pragma once

namespace lcpp
{
    class LispObject;

    namespace lambda
    {
        namespace userDefined
        {
            class Data
            {
            public:

                Ptr<LispObject> getName();
                void setName(Ptr<LispObject> pNewName);

                Ptr<LispObject> getEnv();
                Ptr<LispObject> getArgList();
                Ptr<LispObject> getBody();

            public:

                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pName);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pEnv);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pArgList);
                LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pBody);

            };
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/lambdaData_userDefined.inl"
