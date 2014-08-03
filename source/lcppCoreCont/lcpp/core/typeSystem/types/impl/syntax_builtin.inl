
namespace lcpp
{
    namespace syntax
    {
        namespace builtin
        {
            EZ_FORCE_INLINE
            Ptr<LispObject> create(Function_t pFunction)
            {
                return create(pFunction, { 0, Signature::VarArg });
            }

            EZ_FORCE_INLINE
            Ptr<Signature> Data::getSignature()
            {
                return &m_signature;
            }

            EZ_FORCE_INLINE
            Function_t Data::getFunction()
            {
                return m_pFunction;
            }
        }
    }
}
