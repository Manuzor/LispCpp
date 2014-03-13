#pragma once

namespace lcpp
{
    // namespace for all singletons such as bool types and void.
    namespace singletons
    {
        extern LCPP_CORE_API const SchemeVoid g_void;
        extern LCPP_CORE_API const SchemeNil g_nil;
    };
}

// Easy access macros for all singleton instances
#define SCHEME_VOID  ::lcpp::singletons::g_void
#define SCHEME_NIL   ::lcpp::singletons::g_nil