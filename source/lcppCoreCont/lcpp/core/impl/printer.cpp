#include "stdafx.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/void.h"

#include "lcpp/core/ioUtils.h"

namespace lcpp
{
    namespace printer
    {

        StackPtr<LispObject> print(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);
            auto pStack = cont::getStack(pCont);

            if (pStack->isEmpty())
            {
                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            auto pState = cont::getRuntimeState(pCont);

            auto pOutput = pState->getPrinterState()->m_pOutStream;

            auto pToPrint = pStack->get(-1);
            pStack->pop();

            if(object::isType(pToPrint, Type::String))
            {
                *pOutput << "\"" << str::getValue(pToPrint) << "\"";
            }
            else
            {
                auto pStringObject = object::toString(pToPrint);
                *pOutput << str::getValue(pStringObject);
            }

            LCPP_cont_tailCall(pCont);
        }

        StackPtr<LispObject> lineBreak(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);
            auto pState = cont::getRuntimeState(pCont);
            auto pOutput = pState->getPrinterState()->m_pOutStream;
            *pOutput << "\n";

            LCPP_cont_return(pCont, LCPP_pVoid);
        }

    }
}
