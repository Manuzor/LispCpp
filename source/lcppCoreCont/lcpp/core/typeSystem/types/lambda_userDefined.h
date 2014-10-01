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

            LCPP_API_CORE_CONT StackPtr<LispObject> create(StackPtr<LispObject> pParentEnv, StackPtr<LispObject> pArgList, StackPtr<LispObject> pBodyList);

            LCPP_API_CORE_CONT StackPtr<LispObject> call(StackPtr<LispObject> pCont);

            LCPP_API_CORE_CONT StackPtr<LispObject> getName(Ptr<LispObject> pLambda);
            LCPP_API_CORE_CONT void setName(StackPtr<LispObject> pLambda, StackPtr<LispObject> pNewName);
            LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> getEnvironment(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> getArgList(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> getBody(StackPtr<LispObject> pLambda);

            LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

            namespace detail
            {
                StackPtr<LispObject> call_updateEnv(StackPtr<LispObject> pCont);
                StackPtr<LispObject> call_evalBody(StackPtr<LispObject> pCont);
                StackPtr<LispObject> call_finalize(StackPtr<LispObject> pCont);
            }
        }
    }
}
