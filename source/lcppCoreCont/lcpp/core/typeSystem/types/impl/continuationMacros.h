
#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)

    #undef LCPP_ContinuationDebug
    #define LCPP_ContinuationDebug EZ_ON

#endif

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_trampoline
#define LCPP_cont_trampoline(pCont) \
    return pCont

#if EZ_ENABLED(LCPP_ContinuationDebug)

    /// This trick is used to build a c-stack for debugging even when using continuations.
    #undef LCPP_cont_trampoline
#define LCPP_cont_trampoline(pCont)  \
    ::lcpp::cont::trampoline(pCont); \
    return LCPP_pNil

#endif

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_return
#define LCPP_cont_return(pCont) \
    LCPP_cont_trampoline(::lcpp::cont::getParent(pCont))

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_tailCall
#define LCPP_cont_tailCall(pCont, pFunction)     \
    ::lcpp::cont::setFunction(pCont, pFunction); \
    LCPP_cont_trampoline(pCont)

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_jump
#define LCPP_cont_jump(pCont, pFunction) LCPP_cont_tailCall(pCont, pFunction)
