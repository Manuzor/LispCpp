#include "lcpp/core/exceptions/typeCheckFailedException.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    void typeCheckFailed(const char* szMessage)
    {
        LCPP_THROW(exceptions::TypeCheckFailed(szMessage));
    }

#if EZ_DISABLED(LCPP_RUNTIME_TYPE_CHECK)

    EZ_FORCE_INLINE
    void typeCheck(const Ptr<LispObject>&, const Type&) {}

    EZ_FORCE_INLINE
    void typeCheck(const Ptr<LispObject>&, Type::Enum) {}

#endif
}
