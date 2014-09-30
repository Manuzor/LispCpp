
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        StackPtr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            return getQualifiedName(pObject);
        }
    };
}
