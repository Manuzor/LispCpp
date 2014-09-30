#include "lcpp/core/functionUtils/signature.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            EZ_FORCE_INLINE
            Ptr<Signature> Data::getSignature()
            {
                return &m_signature;
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getName()
            {
                return reinterpret_cast<StackPtr<LispObject>&>(m_pName);
            }

            EZ_FORCE_INLINE
            void Data::setName(StackPtr<LispObject> pNewName)
            {
                reinterpret_cast<Ptr<LispObject>&>(m_pName) = pNewName.get();
            }

            EZ_FORCE_INLINE
            Ptr<LispObject> Data::getEnv()
            {
                return reinterpret_cast<Ptr<LispObject>&>(m_pEnv);
            }

            EZ_FORCE_INLINE
            Function_t Data::getFunction()
            {
                return m_pFunction;
            }
        }
    }
}
