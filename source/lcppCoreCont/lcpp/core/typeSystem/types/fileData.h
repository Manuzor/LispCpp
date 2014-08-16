#pragma once

namespace lcpp
{
    namespace file
    {
        class Data
        {
        public:

            ezOSFile& getFile();
            const ezOSFile& getFile() const;

            Ptr<LispObject> getFileName();
            void setFileName(Ptr<LispObject> pValue);

        public:

            LCPP_DeclareRawDataMember(ezOSFile, m_file);
            LCPP_DeclareRawDataMember(Ptr<LispObject>, m_pFileName);
        };
    }
}

#include "lcpp/core/typeSystem/types/impl/fileData.inl"
