#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"
#include "lcpp/core/exceptions/invalidInputException.h"
#include "lcpp/core/typeSystem/types/syntax_builtin.h"
#include "lcpp/core/typeSystem/types/syntaxData_builtin.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/symbol.h"

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            Ptr<const MetaInfo> getMetaInfo()
            {
                static auto meta = []
                {
                    auto meta = MetaInfo();
                    meta.setType(Type::Syntax);
                    meta.setAttributes(AttributeFlags::Callable
                                       | AttributeFlags::Builtin
                                       | AttributeFlags::Nameable);
                    meta.setPrettyName("builtin-syntax");
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::CallFunction, &call));

                    return meta;
                }(); // Note that this lambda is immediately called.

                return &meta;
            }

            Ptr<LispObject> create(Function_t pFunction, const Signature& signature)
            {
                LCPP_LogBlock("syntax::builtin::create");

                EZ_ASSERT(pFunction, "Invalid function pointer.");
                EZ_ASSERT(signature.m_argCountMin <= signature.m_argCountMax, "Invalid virtual signature.");

                auto pInstance = object::create<Data>(getMetaInfo());
                auto& data = pInstance->getData<Data>();

                data.m_signature = signature;
                data.m_pName = LCPP_pNil;
                data.m_pFunction = pFunction;

                return pInstance;
            }

            Ptr<LispObject> call(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pSyntax = pStack->get(-1);
                pStack->pop();

                checkArguments(pSyntax, pCont);

                auto pFunction = getFunction(pSyntax);

                LCPP_cont_tailCall(pCont, pFunction);
            }

            void checkArguments(Ptr<LispObject> pSyntax, Ptr<LispObject> pCont)
            {
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                EZ_ASSERT(pStack->size() >= 1, "The environment has to be there ALWAYS, even on invalid input.");

                typeCheck(pStack->get(0), Type::Environment);

                auto argCount = pStack->size() - 1;
                auto pSignature = getSignature(pSyntax);
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
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                return &pSyntax->getData<Data>().m_signature;
            }

            Ptr<LispObject> getName(Ptr<LispObject> pSyntax)
            {
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                return pSyntax->getData<Data>().m_pName;
            }

            void setName(Ptr<LispObject> pSyntax, Ptr<LispObject> pNewName)
            {
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);
                typeCheck(pNewName, Type::Symbol);

                pSyntax->getData<Data>().m_pName = pNewName;
            }

            bool hasName(Ptr<LispObject> pSyntax)
            {
                return !isNil(getName(pSyntax));
            }

            Function_t getFunction(Ptr<LispObject> pSyntax)
            {
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                return pSyntax->getData<Data>().m_pFunction;
            }

            Ptr<LispObject> toString(StackPtr<LispObject> pObject)
            {
                typeCheck(pObject, Type::Syntax);
                attributeCheckAny(pObject, AttributeFlags::Builtin);

                auto theString = ezStringBuilder();
                theString.AppendFormat("<%s", getMetaInfo()->getPrettyName());

                if(hasName(pObject))
                {
                    theString.AppendFormat(": %s", symbol::getValue(getName(pObject)).GetData());
                }

                theString.Append('>');

                return str::create(theString);
            }

        }
    }
}
