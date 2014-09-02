#include "stdafx.h"
#include "lcpp/core/runtime.h"

#include "lcpp/core/typeSystem/types/lambda_builtin.h"
#include "lcpp/core/typeSystem/types/lambdaData_builtin.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"
#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/symbol.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            const MetaInfo& metaInfo()
            {
                static auto meta = []
                {
                    auto meta = MetaInfo();
                    meta.setType(Type::Lambda);
                    meta.setAttributes(AttributeFlags::Callable
                                     | AttributeFlags::Builtin
                                     | AttributeFlags::Nameable
                                     | AttributeFlags::EnvironmentContainer);
                    meta.setPrettyName("builtin-procedure");
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::CallFunction, &call));
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::ToStringFunction, &toString));

                    return meta;
                }(); // Note that this lambda is immediately called.

                return meta;
            }

            Ptr<LispObject> create(Ptr<LispObject> pParentEnv,Function_t pFunction, const Signature& signature)
            {
                LCPP_LogBlock("lambda::builtin::create");

                typeCheck(pParentEnv, Type::Environment);

                auto pInstance = object::create<Data>(metaInfo());
                auto& data = pInstance->getData<Data>();

                auto pLocalEnv = env::createAnonymous(pParentEnv);

                data.m_signature = signature;
                new (data.m_pName) Ptr<LispObject>(LCPP_pNil);
                new (data.m_pEnv) Ptr<LispObject>(pLocalEnv);
                data.m_pFunction = pFunction;

                return pInstance;
            }

            Ptr<LispObject> call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pState = cont::getRuntimeState(pCont);
                auto pStack = cont::getStack(pCont);
                auto pLambda = pStack->get(-1);
                pStack->pop();

                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                checkArguments(pLambda, pCont);

                auto pFunction = getFunction(pLambda);

                auto pContCall = cont::create(pCont, pFunction);
                auto pStackCall = cont::getStack(pContCall);

                for(auto i = ezUInt32(0); i < pStack->size(); ++i)
                {
                    auto pToPush = pStack->get(i);
                    pStackCall->push(pToPush);
                }

                pState->increaseRecursionDepth();

                cont::setFunction(pCont, &detail::call_finalize);
                LCPP_cont_jump(pContCall);
            }

            Ptr<LispObject> detail::call_finalize(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pState = cont::getRuntimeState(pCont);
                pState->decreaseRecursionDepth();

                auto pStack = cont::getStack(pCont);
                LCPP_cont_return(pCont, pStack->get(-1));
            }

            void checkArguments(Ptr<LispObject> pLambda, Ptr<LispObject> pCont)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto argCount = pStack->size() - 1;
                auto pSignature = getSignature(pLambda);
                auto argMin = pSignature->m_argCountMin;
                auto argMax = pSignature->m_argCountMax;

                if(argMin == Signature::VarArg
                   || argCount >= argMin && argCount <= argMax)
                {
                    return;
                }

                auto format = "Expected %s arguments, got %d";
                auto message = ezStringBuilder();
                message.Append("Expected ");

                if(argMax == Signature::VarArg)
                {
                    message.AppendFormat("at least %d ", argMin);
                }
                else if(argMin != argMax)
                {
                    message.AppendFormat("%d - %d ", argMin, argMax);
                }
                else
                {
                    // Fixed number of args.
                    message.AppendFormat("%d ", argMin);
                }

                message.AppendFormat("arguments, got %d.", argCount);
                LCPP_THROW(exceptions::ArgumentCount(message.GetData()));
            }

            Ptr<Signature> getSignature(Ptr<LispObject> pSyntax)
            {
                typeCheck(pSyntax, Type::Lambda);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                return pSyntax->getData<Data>().getSignature();
            }

            Ptr<LispObject> getName(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().getName();
            }

            void setName(Ptr<LispObject> pLambda, Ptr<LispObject> pNewName)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);
                typeCheck(pNewName, Type::Symbol);

                pLambda->getData<Data>().setName(pNewName);
            }

            bool hasName(Ptr<LispObject> pLambda)
            {
                return !isNil(getName(pLambda));
            }

            Ptr<LispObject> getEnvironment(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().getEnv();
            }

            Function_t getFunction(Ptr<LispObject> pLambda)
            {
                typeCheck(pLambda, Type::Lambda);
                attributeCheckAny(pLambda, AttributeFlags::Builtin);

                return pLambda->getData<Data>().getFunction();
            }

            Ptr<LispObject> toString(Ptr<LispObject> pObject)
            {
                typeCheck(pObject, Type::Lambda);
                attributeCheckAny(pObject, AttributeFlags::Builtin);

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
