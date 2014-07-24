#pragma once

namespace lcpp
{
    class LispObject;
    
    class LCPP_CORE_API Printer
    {
    public:
        Printer();
        virtual ~Printer();

        virtual void print(Ptr<LispObject> object);
        virtual void print(const std::string& object);
        virtual void print(const ezStringBuilder& str);
        virtual void print(const String& str);
        virtual void print(const char* str);

        virtual void setOutputStream(std::ostream& out);

    private:
        Ptr<std::ostream> m_pOut;
    };
}
