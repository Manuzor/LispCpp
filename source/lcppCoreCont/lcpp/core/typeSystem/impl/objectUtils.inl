
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isCallable(Ptr<LispObject> pObject)
    {
        return pObject->getMetaInfo().getAttributes().isCallable();
    }

    EZ_FORCE_INLINE
    bool isBuiltin(Ptr<LispObject> pObject)
    {
        return pObject->getMetaInfo().getAttributes().isBuiltin();
    }
}
