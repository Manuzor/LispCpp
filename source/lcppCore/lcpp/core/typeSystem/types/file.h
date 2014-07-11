#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeFile :
        public SchemeExtend<SchemeFile, SchemeObject>
    {
        friend class TypeFactory;
    public:
        SchemeFile(const SchemeFile& toCopy);
        virtual ~SchemeFile();

        virtual bool operator ==(const SchemeObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        Ptr<SchemeBool> open(Ptr<SchemeObject> pMode);
        Ptr<SchemeBool> isOpen() const;
        void close();

        ezString readString();

        const ezString& name() const;
        const ezOSFile& file() const;

    private:
        const ezString m_fileName;
        ezOSFile m_file;
        ezStringBuilder m_content;
        ezFileMode::Enum m_lastFileMode;

        SchemeFile(const char* szFileName);
        SchemeFile(const ezString& fileName);

        bool doOpen();

        LCPP_DISALLOW_COPY_ASSIGNMENT(SchemeFile);
    };

    DECLARE_SCHEME_TYPE_INFO(SchemeFile);

    bool operator ==(const SchemeFile& lhs, const SchemeFile& rhs);

}

#include "lcpp/core/typeSystem/types/implementation/file_inl.h"
