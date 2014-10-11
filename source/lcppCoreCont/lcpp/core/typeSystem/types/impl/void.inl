
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isVoid(Ptr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_pVoid;
    }
}
