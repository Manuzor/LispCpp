#include "stdafx.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/types/lambdaData_userDefined.h"
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
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/cons.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/symbol.h"

namespace lcpp
{
    namespace lambda
    {
        namespace userDefined
        {
            Ptr<const MetaInfo> getMetaInfo()
            {
                static auto meta = []
                {
                    auto meta = MetaInfo();
                    meta.setType(Type::Lambda);
                    meta.setAttributes(AttributeFlags::Callable
                                       | AttributeFlags::Nameable
                                       | AttributeFlags::EnvironmentContainer);
                    meta.setPrettyName("procedure");
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::CallFunction, &call));

                    return meta;
                }(); // Note that this lambda is immediately called.

                return &meta;
            }

            Ptr<LispObject> create(StackPtr<LispObject> pParentEnv, StackPtr<LispObject> pArgList, StackPtr<LispObject> pBodyList)
            {
                LCPP_LogBlock("lambda::userDefined::create");

                typeCheck(pParentEnv, Type::Environment);

                auto pInstance = object::create<Data>(getMetaInfo());
                auto& data = pInstance->getData<Data>();

                auto pLocalEnv = env::createAnonymous(pParentEnv);

                static auto pBegin = syntax::builtin::create(&syntax::builtin::begin,
                                                             Signature::createVarArg());

                pBodyList = cons::create(pBegin, pBodyList);

                data.m_pName = LCPP_pNil;
                data.m_pEnv = pLocalEnv;
                data.m_pArgList = pArgList.get();
                data.m_pBody = pBodyList.get();

                while(!isNil(pArgList))
                {
                    auto pCurrentArgName = cons::getCar(pArgList);
                    env::addBinding(pLocalEnv, pCurrentArgName, LCPP_pNil);

                    pArgList = cons::getCdr(pArgList);
                }

                return pInstance;
            }

            Ptr<LispObject> call(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> detail::call_updateEnv(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> detail::call_evalBody(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> detail::call_finalize(StackPtr<LispObject> pCont)
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

                return pLambda->getData<Data>().m_pName;
            }

            void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);
                typeCheck(pNewName, Type::Symbol);

                pLambda->getData<Data>().m_pName = pNewName;
            }

            bool hasName(Ptr<LispObject> pLambda)
            {
                return !isNil(getName(pLambda));
            }

            Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().m_pEnv;
            }

            Ptr<LispObject> getArgList(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().m_pArgList;
            }

            Ptr<LispObject> getBody(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().m_pBody;
            }

            Ptr<LispObject> toString(StackPtr<LispObject> pObject)
            {
                typeCheck(pObject, Type::Lambda);
                attributeCheckNone(pObject, AttributeFlags::Builtin);

                auto theString = ezStringBuilder();
                theString.AppendFormat("<%s", getMetaInfo()->getPrettyName());

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
