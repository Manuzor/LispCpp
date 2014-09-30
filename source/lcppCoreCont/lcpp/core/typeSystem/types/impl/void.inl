
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isVoid(StackPtr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_pVoid;
    }
}
