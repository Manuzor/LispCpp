#include "stdafx.h"
#include "lcpp/core/builtins/syntax_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/continuation.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/evaluator.h"
#include "lcpp/core/typeSystem/types/environment.h"
#include "lcpp/core/typeSystem/types/void.h"

#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            Ptr<LispObject> define(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto pEnv = pStack->get(0);
                typeCheck(pEnv, Type::Environment);

                auto pSymbol = pStack->get(1);
                typeCheck(pSymbol, Type::Symbol);

                auto pValue = pStack->get(2);
                // Pop the unevaluated value so eval::evaluate can push the evaluated value.
                pStack->pop();

                //////////////////////////////////////////////////////////////////////////

                cont::setFunction(pCont, &define_2);
                LCPP_cont_call(pCont, &eval::evaluate, pEnv, pValue);
            }

            Ptr<LispObject> define_2(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pStack = cont::getStack(pCont);

                auto pEnv = pStack->get(0);
                typeCheck(pEnv, Type::Environment);

                auto pSymbol = pStack->get(1);
                typeCheck(pSymbol, Type::Symbol);

                auto pValue = pStack->get(2);

                //////////////////////////////////////////////////////////////////////////

                env::addBinding(pEnv, pSymbol, pValue);

                // TODO give pValue a name, if it is nameable.

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

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
