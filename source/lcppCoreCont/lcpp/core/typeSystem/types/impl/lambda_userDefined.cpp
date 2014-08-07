#include "stdafx.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

#include "lcpp/core/evaluator.h"

namespace lcpp
{
    namespace lambda
    {
        namespace userDefined
        {
            const MetaInfo& metaInfo()
            {
                static auto meta = MetaInfo(Type::Lambda,
                                            AttributeFlags::Callable | AttributeFlags::Nameable,
                                            "procedure");

                return meta;
            }

            Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Ptr<LispObject> pArgList, Ptr<LispObject> pBodyList)
            {
                typeCheck(pParentEnv, Type::Environment);

                auto pInstance = object::create<Data>(metaInfo());
                auto& data = pInstance->m_lambda_userDefined;

                auto pLocalEnv = env::createAnonymous(pParentEnv);

                new (data.m_pName) Ptr<LispObject>(LCPP_pNil);
                new (data.m_pEnv) Ptr<LispObject>(pLocalEnv);
                new (data.m_pArgList) Ptr<LispObject>(pArgList);
                new (data.m_pBodyList) Ptr<LispObject>(pBodyList);

                while(!isNil(pArgList))
                {
                    auto pCurrentArgName = cons::getCar(pArgList);
                    env::addBinding(pLocalEnv, pCurrentArgName, LCPP_pNil);

                    pArgList = cons::getCdr(pArgList);
                }

                return pInstance;
            }

            Ptr<LispObject> call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pLambda = pStack->get(-1);

                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                auto pArgList = getArgList(pLambda);
                pStack->push(pArgList);

                // Set the current 'index' to 0.
                cont::setUserData(pCont, 0);

                LCPP_cont_tailCall(pCont, &detail::call_updateEnv);
            }

            Ptr<LispObject> getName(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_userDefined.getName();
            }

            void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);
                typeCheck(pNewName, Type::Symbol);

                pLambda->m_lambda_userDefined.setName(pNewName);
            }

            bool hasName(Ptr<LispObject> pLambda)
            {
                return !isNil(getName(pLambda));
            }

            Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_userDefined.getEnv();
            }

            Ptr<LispObject> getArgList(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_userDefined.getArgList();
            }

            Ptr<LispObject> getBodyList(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_userDefined.getBodyList();
            }

            Ptr<LispObject> toString(Ptr<LispObject> pObject)
            {
                typeCheck(pObject, Type::Lambda);
                attributeCheckNone(pObject, AttributeFlags::Builtin);

                auto theString = ezStringBuilder();
                theString.AppendFormat("<%s", metaInfo().getPrettyName().GetData());

                if (hasName(pObject))
                {
                    theString.AppendFormat(": %s", symbol::getValue(getName(pObject)).GetData());
                }

                theString.Append('>');

                return str::create(theString);
            }

            namespace detail
            {
                Ptr<LispObject> call_updateEnv(Ptr<LispObject> pCont)
                {
                    typeCheck(pCont, Type::Continuation);

                    auto pStack = cont::getStack(pCont);
                    auto& index = cont::getUserData(pCont);

                    auto pLambda = pStack->get(-2);
                    auto& pCurrentArgNameList = pStack->get(-1);

                    const auto maxIndex = pStack->size() - 2;

                    if (isNil(pCurrentArgNameList))
                    {
                        pStack->clear();
                        pStack->push(pLambda);

                        LCPP_cont_tailCall(pCont, &call_evalBody);
                    }

                    auto pEnv = getEnvironment(pLambda);

                    auto pCurrentArgName = cons::getCar(pCurrentArgNameList);
                    auto pCurrentArgValue = index < maxIndex ? pStack->get(ezInt32(index)) : LCPP_pNil;

                    auto result = env::setBinding(pEnv, pCurrentArgName, pCurrentArgValue);
                    if (result.Failed())
                    {
                        LCPP_NOT_IMPLEMENTED;
                    }

                    ++index;
                    pCurrentArgNameList = cons::getCdr(pCurrentArgNameList);

                    LCPP_cont_tailCall(pCont, &call_updateEnv);
                }

                Ptr<LispObject> call_evalBody(Ptr<LispObject> pCont)
                {
                    typeCheck(pCont, Type::Continuation);

                    auto pStack = cont::getStack(pCont);
                    auto pLambda = pStack->get(-1);
                    pStack->clear();

                    auto pBodyList = getBodyList(pLambda);

                    pStack->push(pLambda);                   // The lambda object itself, needed for its environment.
                    cons::pushAllReverse(pBodyList, pStack); // All body-parts to evaluate in reverse order so the first to eval is on top.
                    pStack->push(LCPP_pNil);                 // Needed by call_evalBody_helper.

                    LCPP_cont_tailCall(pCont, &call_evalBody_2);
                }

                Ptr<LispObject> call_evalBody_2(Ptr<LispObject> pCont)
                {
                    typeCheck(pCont, Type::Continuation);

                    auto pStack = cont::getStack(pCont);
                    auto pEvalResult = pStack->get(-1);
                    pStack->pop();

                    EZ_ASSERT(pStack->size() > 0, "Invalid stack state.");

                    if (pStack->size() == 1)
                    {
                        LCPP_cont_return(pCont, pEvalResult);
                    }

                    auto pToEval = pStack->get(-1);
                    pStack->pop();

                    auto pLambda = pStack->get(0);
                    auto pEnv = getEnvironment(pLambda);

                    LCPP_cont_call(pCont, &eval::evaluate, pEnv, pToEval);
                }

            }
        }
    }
}
