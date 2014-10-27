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

            LCPP_API_CORE_CONT Ptr<LispObject> create(StackPtr<LispObject> pParentEnv, StackPtr<LispObject> pArgList, StackPtr<LispObject> pBodyList);

            LCPP_API_CORE_CONT Ptr<LispObject> call(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getArgList(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> getBody(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);

            namespace detail
            {
                Ptr<LispObject> call_updateEnv(StackPtr<LispObject> pCont);
                Ptr<LispObject> call_evalBody(StackPtr<LispObject> pCont);
            }
        }
    }
}
