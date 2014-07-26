
namespace lcpp
{
    inline
    bool isTrue(Ptr<LispObject> pObject)
    {
        return !isFalse(pObject);
    }

    inline
    bool isFalse(Ptr<LispObject> pObject)
    {
        return pObject == LCPP_pFalse;
    }

}
