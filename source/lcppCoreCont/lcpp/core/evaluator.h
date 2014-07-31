#pragma once

namespace lcpp
{
    namespace eval
    {
        /// \brief Evaluates a given LispObject
        /// \remark Continuation function.
        /// Expects the following arguments:
        /// [0]: Ptr<LispObject> to eval.
        LCPP_API_CORE_CONT Ptr<LispObject> evaluate(Ptr<LispObject> pContinuation);
    }
}
