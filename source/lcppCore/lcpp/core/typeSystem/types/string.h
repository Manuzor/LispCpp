#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LCPP_CORE_API LispString:
        public LispObject
    {
        friend class TypeFactory;
    public:

        static Ptr<LispString> create(const ezString& value);
        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;
        virtual const Type& type() const LCPP_OVERRIDE;
        virtual bool operator ==(const LispObject& rhs) const LCPP_OVERRIDE;
        virtual ezString toString() const LCPP_OVERRIDE;

        operator ezString() const;

        const ezString& value() const;
        void value(const ezString& newSymbol);
        void value(const char* newSymbol);

    private:
        ezString m_string;

        LispString(const char* str);
        LispString(const ezString& str);
    };

    bool operator ==(const LispString& lhs, const LispString& rhs);

    bool operator ==(const LispString& lhs, const char* rhs);
    bool operator ==(const char* lhs, const LispString& rhs);

    bool operator ==(const LispString& lhs, const ezString& rhs);
    bool operator ==(const ezString& lhs, const LispString& rhs);

}


#include "lcpp/core/typeSystem/types/implementation/string.inl"
