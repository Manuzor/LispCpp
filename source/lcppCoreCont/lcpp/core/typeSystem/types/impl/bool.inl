
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isTrue(StackPtr<LispObject> pObject)
    {
        return !isFalse(pObject);
    }

    EZ_FORCE_INLINE
    bool isFalse(StackPtr<LispObject> pObject)
    {
        return pObject == LCPP_pFalse;
    }

}
