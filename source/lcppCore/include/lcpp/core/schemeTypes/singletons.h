#pragma once

namespace lcpp
{
    // namespace for all singletons such as bool types and void.
    namespace singletons
    {
        extern LCPP_CORE_API const SchemeVoid g_void;
        extern LCPP_CORE_API const SchemeNil g_nil;
        extern LCPP_CORE_API const SchemeBool g_true;
        extern LCPP_CORE_API const SchemeBool g_false;
    };
}

// Easy access macros for all singleton instances
#define SCHEME_VOID  ::lcpp::singletons::g_void
#define SCHEME_NIL   ::lcpp::singletons::g_nil
#define SCHEME_TRUE  ::lcpp::singletons::g_true
#define SCHEME_FALSE ::lcpp::singletons::g_false