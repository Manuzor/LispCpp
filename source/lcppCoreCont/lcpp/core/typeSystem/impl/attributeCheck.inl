#include "lcpp/core/exceptions/typeCheckFailedException.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    void attributeCheckFailed(const char* szMessage)
    {
        LCPP_THROW(exceptions::TypeCheckFailed("Invalid attribute flags."));
    }

#if EZ_DISABLED(LCPP_RUNTIME_TYPE_CHECK)

    EZ_FORCE_INLINE
    void attributeCheckAll(const StackPtr<LispObject>&, const AttributeFlags& expectedFlags) {}

    EZ_FORCE_INLINE
    void attributeCheckAny(const StackPtr<LispObject>&, const AttributeFlags& expectedFlags) {}

    EZ_FORCE_INLINE
    void attributeCheckAll(const StackPtr<LispObject>&, ezUInt32) {}

    EZ_FORCE_INLINE
    void attributeCheckAny(const StackPtr<LispObject>&, ezUInt32) {}

#endif
}
