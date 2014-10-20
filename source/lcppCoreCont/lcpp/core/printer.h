#pragma once
#include "lcpp/core/printerState.h"

namespace lcpp
{
    namespace printer
    {
        /// \brief Prints all objects on the stack.
        LCPP_API_CORE_CONT Ptr<LispObject> print(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispObject> lineBreak(StackPtr<LispObject> pCont);
    }
}
