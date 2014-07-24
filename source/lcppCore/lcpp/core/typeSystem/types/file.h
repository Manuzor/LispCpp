#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispBool;

    class LCPP_API_CORE LispFile :
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispFile> create(const String& fileName);
        static Ptr<LispFile> copy(const LispFile& toCopy);

        static const Type& typeInfo();

    public:

        LispFile(const LispFile& toCopy);
        virtual ~LispFile();

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual String toString() const LCPP_OVERRIDE;

        Ptr<LispBool> open(Ptr<LispObject> pMode);
        Ptr<LispBool> isOpen() const;
        void close();

        String readString();

        const String& name() const;
        const ezOSFile& file() const;

    private:
        const String m_fileName;
        ezOSFile m_file;
        ezStringBuilder m_content;
        ezFileMode::Enum m_lastFileMode;

        LispFile(const char* szFileName);
        LispFile(const String& fileName);

        bool doOpen();

        LCPP_DISALLOW_COPY_ASSIGNMENT(LispFile);
    };

    bool operator ==(const LispFile& lhs, const LispFile& rhs);

}

#include "lcpp/core/typeSystem/types/implementation/file.inl"
