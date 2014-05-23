#pragma once

namespace lcpp
{
    class SchemeObject;

    class LCPP_CORE_API IPrinter
    {
    public:
        virtual ~IPrinter() {}

        virtual void print(SchemeObject& object) = 0;

        virtual void setOutputStream(std::ostream& out) = 0;
    };
    
    class LCPP_CORE_API Printer : public IPrinter
    {
    public:
        Printer();
        virtual ~Printer();

        virtual void print(SchemeObject& object) override;
        virtual void setOutputStream(std::ostream& out) override;

    private:
        std::ostream* m_out;
    };
}
