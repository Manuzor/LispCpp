
namespace lcpp
{
    namespace file
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> open(StackPtr<LispObject> pFile, StackPtr<LispObject> pStringFileMode)
        {
            return open(pFile, getFileName(pFile), pStringFileMode);
        }
    }
}
