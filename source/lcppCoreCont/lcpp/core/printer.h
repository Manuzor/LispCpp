#pragma once
#include "lcpp/core/printerState.h"

namespace lcpp
{
    namespace printer
    {
        /// \brief Prints all objects on the stack.
        LCPP_API_CORE_CONT Ptr<LispObject> print(Ptr<LispObject> pCont);
    }
}
