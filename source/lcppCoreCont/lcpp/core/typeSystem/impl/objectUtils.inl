
namespace lcpp
{
    EZ_FORCE_INLINE
    bool isCallable(Ptr<LispObject> pObject)
    {
        return pObject->getMetaInfo().getAttributes().isCallable();
    }
}
