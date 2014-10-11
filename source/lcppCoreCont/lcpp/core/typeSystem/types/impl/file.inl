
namespace lcpp
{
    namespace file
    {
        EZ_FORCE_INLINE
        Ptr<LispObject> open(Ptr<LispObject> pFile, Ptr<LispObject> pStringFileMode)
        {
            return open(pFile, getFileName(pFile), pStringFileMode);
        }
    }
}
