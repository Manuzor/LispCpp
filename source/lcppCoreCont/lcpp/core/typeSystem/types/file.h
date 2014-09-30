#pragma once

namespace lcpp
{
    class MetaInfo;
    class LispObject;
    class LispRuntimeState;

    namespace file
    {
        LCPP_API_CORE_CONT Ptr<const MetaInfo> getMetaInfo();

        LCPP_API_CORE_CONT StackPtr<LispObject> create();

        LCPP_API_CORE_CONT StackPtr<LispObject> open(StackPtr<LispObject> pFile, StackPtr<LispObject> pStringFileMode);
        LCPP_API_CORE_CONT StackPtr<LispObject> open(StackPtr<LispObject> pFile, StackPtr<LispObject> pStringFileName, StackPtr<LispObject> pStringFileMode);
        LCPP_API_CORE_CONT StackPtr<LispObject> isOpen(StackPtr<LispObject> pFile);
        LCPP_API_CORE_CONT void close(StackPtr<LispObject> pFile);

        LCPP_API_CORE_CONT StackPtr<LispObject> getFileName(StackPtr<LispObject> pFile);
        LCPP_API_CORE_CONT void setFileName(StackPtr<LispObject> pFile, StackPtr<LispObject> pFileName);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pFile);
    }
}

#include "lcpp/core/typeSystem/types/impl/file.inl"
