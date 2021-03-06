#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace str
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(const char* szData, std::size_t uiLength);

        EZ_FORCE_INLINE Ptr<LispObject> create(const ezStringIterator& iter) { return create(iter.GetData(), iter.GetElementCount()); }

        template<std::size_t N>
        EZ_FORCE_INLINE Ptr<LispObject> create(const char (&szData)[N]) { return create(szData, N - 1); }

        LCPP_API_CORE_CONT ezStringIterator getValue(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);

    }
}
