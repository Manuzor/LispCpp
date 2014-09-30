#include "lcpp/core/functionUtils/signature.h"

namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            EZ_FORCE_INLINE
            StackPtr<LispObject> create(Function_t pFunction)
            {
                return create(pFunction, { 0, Signature::VarArg });
            }
        }
    }
}
