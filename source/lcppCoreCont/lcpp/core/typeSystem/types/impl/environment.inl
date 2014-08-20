
namespace lcpp
{
    namespace env
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            return getQualifiedName(pObject);
        }
    };
}
