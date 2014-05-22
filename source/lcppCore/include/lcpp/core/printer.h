#pragma once

namespace lcpp
{
    class SchemeObject;

    class IPrinter
    {
    public:
        virtual ~IPrinter();

        virtual void print(SchemeObject& object) = 0;
    };
    
    class Printer : public IPrinter
    {
    public:

        virtual void print(SchemeObject& object) override;
    };
}
