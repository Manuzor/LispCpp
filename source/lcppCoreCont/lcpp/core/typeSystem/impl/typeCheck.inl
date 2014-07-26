
namespace lcpp
{
#if EZ_DISABLED(LCPP_RUNTIME_TYPE_CHECK)

    inline
    void typeCheck(const Ptr<LispObject>& pObject, const Type& expectedType)
    {
    }

#endif
}
