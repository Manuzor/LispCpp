
namespace lcpp
{
    inline
    bool isNil(Ptr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_pNil;
    }
}
