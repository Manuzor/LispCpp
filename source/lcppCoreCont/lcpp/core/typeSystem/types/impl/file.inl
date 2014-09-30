
namespace lcpp
{
    namespace file
    {
        EZ_FORCE_INLINE
        StackPtr<LispObject> open(StackPtr<LispObject> pFile, StackPtr<LispObject> pStringFileMode)
        {
            return open(pFile, getFileName(pFile), pStringFileMode);
        }
    }
}
