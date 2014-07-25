
namespace lcpp
{
    inline
    bool isVoid(Ptr<LispObject> pObject)
    {
        // Idenity check.
        return pObject == LCPP_VOID;
    }
}
