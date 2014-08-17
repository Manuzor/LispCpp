#include "stdafx.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"

namespace lcpp
{
    namespace lambda
    {
        namespace userDefined
        {
            const MetaInfo& metaInfo()
            {
                static auto meta = MetaInfo(Type::Lambda,
                                            AttributeFlags::Callable | AttributeFlags::Nameable | AttributeFlags::EnvironmentContainer,
                                            "procedure");

                return meta;
            }

            Ptr<LispObject> create(Ptr<LispObject> pParentEnv, Ptr<LispObject> pArgList, Ptr<LispObject> pBodyList)
            {
                typeCheck(pParentEnv, Type::Environment);

                auto pInstance = object::create<Data>(metaInfo());
                auto& data = pInstance->m_lambda_userDefined;

                auto pLocalEnv = env::createAnonymous(pParentEnv);

                static auto pBegin = syntax::builtin::create(&syntax::builtin::begin,
                                                             Signature::createVarArg());

                pBodyList = cons::create(pBegin, pBodyList);

                new (data.m_pName) Ptr<LispObject>(LCPP_pNil);
                new (data.m_pEnv) Ptr<LispObject>(pLocalEnv);
                new (data.m_pArgList) Ptr<LispObject>(pArgList);
                new (data.m_pBody) Ptr<LispObject>(pBodyList);

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
                auto pState = cont::getRuntimeState(pCont);
                pState->increaseRecursionDepth();

                auto pEnv = pStack->get(0);
                typeCheck(pEnv, Type::Environment);

                auto pLambda = pStack->get(-1);

                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                auto pArgList = getArgList(pLambda);
                pStack->push(pArgList);

                // Set the current 'index' to 1.
                cont::setUserData(pCont, 1);

                LCPP_cont_tailCall(pCont, &detail::call_updateEnv);
            }

            Ptr<LispObject> detail::call_updateEnv(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto& index = cont::getUserData(pCont);

                auto pEnv = pStack->get(0);
                typeCheck(pEnv, Type::Environment);

                auto pLambda = pStack->get(-2);
                typeCheck(pLambda, Type::Lambda);

                auto& pCurrentArgNameList = pStack->get(-1);

                const auto maxIndex = pStack->size() - 3;

                if(isNil(pCurrentArgNameList))
                {
                    pStack->clear();
                    pStack->push(pEnv);
                    pStack->push(pLambda);

                    LCPP_cont_tailCall(pCont, &call_evalBody);
                }

                auto pCurrentArgName = cons::getCar(pCurrentArgNameList);
                auto pCurrentArgValue = index <= maxIndex ? pStack->get(index) : LCPP_pNil;

                env::addBinding(pEnv, pCurrentArgName, pCurrentArgValue);

                ++index;
                pCurrentArgNameList = cons::getCdr(pCurrentArgNameList);

                LCPP_cont_tailCall(pCont, &call_updateEnv);
            }

            Ptr<LispObject> detail::call_evalBody(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto pEnv = pStack->get(0);
                typeCheck(pEnv, Type::Environment);
                auto pLambda = pStack->get(-1);
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                auto pBody = getBody(pLambda);

                cont::setFunction(pCont, &call_finalize);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pBody);
            }

            Ptr<LispObject> detail::call_finalize(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont);
                pState->decreaseRecursionDepth();

                LCPP_cont_return(pCont, pStack->get(-1));
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

            Ptr<LispObject> getBody(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->m_lambda_userDefined.getBody();
            }

            Ptr<LispObject> toString(Ptr<LispObject> pObject)
            {
                typeCheck(pObject, Type::Lambda);
                attributeCheckNone(pObject, AttributeFlags::Builtin);

                auto theString = ezStringBuilder();
                theString.AppendFormat("<%s", metaInfo().getPrettyName());

                if (hasName(pObject))
                {
                    theString.AppendFormat(": %s", symbol::getValue(getName(pObject)).GetData());
                }

                theString.Append('>');

                return str::create(theString);
            }
        }
    }
}
