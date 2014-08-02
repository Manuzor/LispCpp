#include "stdafx.h"
#include "lcpp/core/typeSystem/types/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            Ptr<LispObject> quote(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto pUnevaluatedArg = pStack->get(1);

                LCPP_cont_return(pCont, pUnevaluatedArg);
            }

        }
    }
}
