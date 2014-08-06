#include "stdafx.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"

#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"

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

            auto pArg = pStack->get(-1);

            *pOutput << pArg;

            LCPP_cont_return(pCont, LCPP_pVoid);
        }
    }
}
