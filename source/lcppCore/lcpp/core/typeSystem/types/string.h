#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LCPP_CORE_API LispString:
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispString> create(const String& value);
        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual String toString() const LCPP_OVERRIDE;

        operator String() const;

        const String& value() const;
        void value(const String& newSymbol);
        void value(const char* newSymbol);

    private:
        String m_string;

        LispString(const char* str);
        LispString(const String& str);
    };

    bool operator ==(const LispString& lhs, const LispString& rhs);

    bool operator ==(const LispString& lhs, const char* rhs);
    bool operator ==(const char* lhs, const LispString& rhs);

    bool operator ==(const LispString& lhs, const String& rhs);
    bool operator ==(const String& lhs, const LispString& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/string.inl"
