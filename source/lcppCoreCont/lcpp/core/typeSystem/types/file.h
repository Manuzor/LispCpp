#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;
    class LispRuntimeState;

    namespace file
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create();

        LCPP_API_CORE_CONT Ptr<LispObject> open(Ptr<LispObject> pFile, Ptr<LispObject> pStringFileMode);
        LCPP_API_CORE_CONT Ptr<LispObject> open(Ptr<LispObject> pFile, Ptr<LispObject> pStringFileName, Ptr<LispObject> pStringFileMode);
        LCPP_API_CORE_CONT Ptr<LispObject> isOpen(Ptr<LispObject> pFile);
        LCPP_API_CORE_CONT void close(Ptr<LispObject> pFile);

        LCPP_API_CORE_CONT Ptr<LispObject> getFileName(Ptr<LispObject> pFile);
        LCPP_API_CORE_CONT void setFileName(Ptr<LispObject> pFile, Ptr<LispObject> pFileName);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pFile);
    }
}

#include "lcpp/core/typeSystem/types/impl/file.inl"
