#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace lambda
    {
        namespace userDefined
        {
            LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Ptr<LispObject> pArgList, Ptr<LispObject> pBodyList);

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getArgList(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getBody(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

            namespace detail
            {
                Ptr<LispObject> call_updateEnv(Ptr<LispObject> pCont);
                Ptr<LispObject> call_evalBody(Ptr<LispObject> pCont);
                Ptr<LispObject> call_finalize(Ptr<LispObject> pCont);
            }
        }
    }
}
