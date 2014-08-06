#include "stdafx.h"
#include "lcpp/core/typeSystem/types/lambda_userDefined.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

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

                return pInstance;
            }

            Ptr<LispObject> call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pLambda = pStack->get(-1);
                pStack->pop();

                typeCheck(pLambda, Type::Lambda);
                attributeCheckNone(pLambda, AttributeFlags::Builtin);

                LCPP_NOT_IMPLEMENTED;
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
        }
    }
}
