
namespace lcpp
{
    namespace object
    {
        template<typename T_Data>
        inline
        Ptr<LispObject>
        create(const MetaInfo& metaInfo)
        {
            // Helper struct to determine the minimum memory needed for this lisp object using T_Data
            struct LispObjectProxy
            {
                LispObjectHeader h;
                T_Data d;
            };

            auto pAllocator = getCollectingAllocator();

            auto size = sizeof(LispObjectProxy);

            auto pMem = (char*)(LCPP_NEW(pAllocator, LispObjectProxy)());

#if EZ_ENABLED(EZ_COMPILE_FOR_DEBUG)
            memset(pMem, 0xdadadada, size - 1);
            memset(pMem + size - 1, 0xdd, 1);
#endif

            return new (pMem) LispObject(metaInfo);
        }
    }
}
