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

        Ptr<LispObject> print(Ptr<LispObject> pCont)
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

            if(!isVoid(pToPrint))
            {
                if(object::isType(pToPrint, Type::String))
                {
                    *pOutput << "\"" << str::getValue(pToPrint) << "\"\n";
                }
                else
                {
                    auto pStringObject = object::toString(pToPrint);
                    *pOutput << str::getValue(pStringObject) << "\n";
                }
            }

            LCPP_cont_tailCall(pCont);
        }
    }
}
