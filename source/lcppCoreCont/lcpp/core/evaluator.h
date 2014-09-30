#pragma once

namespace lcpp
{
    namespace eval
    {
        /// \brief Evaluates a given LispObject
        /// \remark Continuation function.
        /// Expects the following arguments:
        /// [0]: StackPtr<LispObject> to eval.
        LCPP_API_CORE_CONT StackPtr<LispObject> evaluate(StackPtr<LispObject> pCont);

        namespace detail
        {
            LCPP_API_CORE_CONT StackPtr<LispObject> evaluateCallable(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> evaluateCallable_evalEach(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> evaluateCallable_processEvaluatedArg(StackPtr<LispObject> pCont);
            LCPP_API_CORE_CONT StackPtr<LispObject> evaluateCallable_call(StackPtr<LispObject> pCont);
        }
    }
}
