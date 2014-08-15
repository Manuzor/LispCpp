
namespace lcpp
{
    namespace object
    {
        template<typename T_Data>
        inline
        Ptr<LispObject>
        create(const MetaInfo& metaInfo)
        {
            // TODO This whole function needs more case, most likely...

            // Helper struct to determine the minimum memory needed for this lisp object using T_Data
            struct LispObjectProxy
            {
                LispObjectHeader h;
                T_Data d;
            };

            auto pAllocator = defaultAllocator();

            auto size = sizeof(LispObjectProxy);

            auto pMem = static_cast<void*>(LCPP_NEW(pAllocator, LispObjectProxy)());

            memset(pMem, 0xdadadada, size);

            return new (pMem) LispObject(metaInfo);
        }
    }
}
