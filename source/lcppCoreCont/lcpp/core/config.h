#ifdef LCPP_CORE_CONT_CONFIG_INCLUDED
#error Config file is not supposed to be included more than once!
#endif
#define LCPP_CORE_CONT_CONFIG_INCLUDED

#define LCPP_RUNTIME_TYPE_CHECK_ENABLED EZ_ON

namespace lcpp
{
    namespace number
    {
        typedef ezInt64 Integer_t;
        typedef double Float_t;
    }
}
