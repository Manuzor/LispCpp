
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isNil(Ptr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_pNil;
    }
}
