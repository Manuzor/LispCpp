#pragma once

namespace lcpp
{
    namespace eval
    {
        /// \brief Evaluates a given LispObject
        /// \remark Continuation function.
        /// Expects the following arguments:
        /// [0]: Ptr<LispObject> to eval.
        LCPP_API_CORE_CONT Ptr<LispObject> evaluate(Ptr<LispObject> pCont);

        namespace detail
        {
            LCPP_API_CORE_CONT Ptr<LispObject> evaluateCallable(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> evaluateCallable_evalEach(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> evaluateCallable_processEvaluatedArg(Ptr<LispObject> pCont);
            LCPP_API_CORE_CONT Ptr<LispObject> evaluateCallable_call(Ptr<LispObject> pCont);
        }
    }
}
