
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isTrue(Ptr<LispObject> pObject)
    {
        return !isFalse(pObject);
    }

    EZ_FORCE_INLINE
    bool isFalse(Ptr<LispObject> pObject)
    {
        return pObject == LCPP_pFalse;
    }

}
