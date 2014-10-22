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

        Ptr<LispObject> print(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            if (cont::getStack(pCont)->isEmpty())
            {
                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            auto pState = cont::getRuntimeState(pCont);

            auto pOutput = pState->getPrinterState()->m_pOutStream;

            StackPtr<LispObject> pToPrint = cont::getStack(pCont)->get(-1);
            cont::getStack(pCont)->pop();

            if(object::isType(pToPrint, Type::String))
            {
                *pOutput << "\"" << str::getValue(pToPrint).GetData() << "\"";
            }
            else
            {
                StackPtr<LispObject> pStringObject = object::toString(pToPrint);
                *pOutput << str::getValue(pStringObject).GetData();
            }

            LCPP_cont_tailCall(pCont);
        }

        Ptr<LispObject> lineBreak(StackPtr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);
            auto pState = cont::getRuntimeState(pCont);
            auto pOutput = pState->getPrinterState()->m_pOutStream;
            *pOutput << "\n";

            LCPP_cont_return(pCont, LCPP_pVoid);
        }

    }
}
