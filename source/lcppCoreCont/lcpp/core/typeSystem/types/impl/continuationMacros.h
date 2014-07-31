
#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)

    #undef LCPP_ContinuationDebug
    #define LCPP_ContinuationDebug EZ_ON

#endif

#undef LCPP_cont_trampoline
#if EZ_ENABLED(LCPP_ContinuationDebug)

#define LCPP_cont_trampoline(pCont) return ::lcpp::cont::trampoline(pCont)

#else

#define LCPP_cont_trampoline(pCont) return pCont

#endif

#define LCPP_cont_return(pContinuation)
#define LCPP_cont_call(pContinuation, pFunction)
#define LCPP_cont_tailCall(pContinuation, pFunction)
