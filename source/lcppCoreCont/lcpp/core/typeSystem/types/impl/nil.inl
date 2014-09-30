
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isNil(StackPtr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_pNil;
    }
}
