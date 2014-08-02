#include "stdafx.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/attribute.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/attributeCheck.h"

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            const MetaInfo& metaInfo()
            {
                static auto meta = MetaInfo(Type::Syntax,
                                            AttributeFlags::Callable | AttributeFlags::Builtin,
                                            "builtin-syntax");

                return meta;
            }

            Ptr<LispObject> create(Function_t pFunction)
            {
                EZ_ASSERT(pFunction, "Invalid function pointer.");

                auto pInstance = LispObject::create<Data>(metaInfo());
                auto& data = pInstance->m_syntax_builtin;

                data.m_pFunction = pFunction;

                return pInstance;
            }

            Ptr<LispObject> call(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);
                auto pSyntax = pStack->get(-1);
                pStack->pop();

                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                auto pFunction = getFunction(pSyntax);

                LCPP_cont_tailCall(pCont, pFunction);
            }

            Function_t getFunction(Ptr<LispObject> pSyntax)
            {
                typeCheck(pSyntax, Type::Syntax);
                attributeCheckAny(pSyntax, AttributeFlags::Builtin);

                return pSyntax->m_syntax_builtin.getFunction();
            }
        }
    }
}
