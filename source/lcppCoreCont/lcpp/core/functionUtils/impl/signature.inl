
namespace lcpp
{
    EZ_FORCE_INLINE
    Signature Signature::create(ezUInt32 argMin, ezUInt32 argMax)
    {
        return { argMin, argMax };
    }

    EZ_FORCE_INLINE
    Signature Signature::create(ezUInt32 argCount)
    {
        return { argCount, argCount };
    }
}
