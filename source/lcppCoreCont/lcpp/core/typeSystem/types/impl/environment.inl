
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            return getQualifiedName(pObject);
        }
    };
}
