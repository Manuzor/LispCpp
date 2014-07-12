#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispFile :
        public LispExtend<LispFile, LispObject>
    {
        friend class TypeFactory;
    public:
        LispFile(const LispFile& toCopy);
        virtual ~LispFile();

        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        Ptr<LispBool> open(Ptr<LispObject> pMode);
        Ptr<LispBool> isOpen() const;
        void close();

        ezString readString();

        const ezString& name() const;
        const ezOSFile& file() const;

    private:
        const ezString m_fileName;
        ezOSFile m_file;
        ezStringBuilder m_content;
        ezFileMode::Enum m_lastFileMode;

        LispFile(const char* szFileName);
        LispFile(const ezString& fileName);

        bool doOpen();

        LCPP_DISALLOW_COPY_ASSIGNMENT(LispFile);
    };

    LCPP_DECLARE_SCHEME_TYPE_INFO(LispFile, "file");

    bool operator ==(const LispFile& lhs, const LispFile& rhs);

}

#include "lcpp/core/typeSystem/types/implementation/file_inl.h"
