
#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)

    #undef LCPP_ContinuationDebug
    #define LCPP_ContinuationDebug EZ_OFF

#else

    #undef LCPP_ContinuationDebug
    #define LCPP_ContinuationDebug EZ_OFF

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
#define LCPP_cont_return(...) \
    LCPP_cont_trampoline(::lcpp::cont::detail::returnHelper(__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_call
#define LCPP_cont_call(...) \
    LCPP_cont_trampoline(::lcpp::cont::detail::callHelper(__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_tailCall
#define LCPP_cont_tailCall(...) \
        LCPP_cont_trampoline(::lcpp::cont::detail::tailCallHelper(__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////

#undef LCPP_cont_jump
#define LCPP_cont_jump(...) LCPP_cont_tailCall(__VA_ARGS__)
