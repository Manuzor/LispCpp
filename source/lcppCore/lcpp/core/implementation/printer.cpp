#include "stdafx.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/typeSystem.h"
#include <iostream>
#include <string>

lcpp::Printer::Printer() :
    m_pOut(&std::cout)
{
}

lcpp::Printer::~Printer()
{
}

void
lcpp::Printer::print(Ptr<LispObject> object)
{
    if(object->is<LispVoid>()){ return; }

    *m_pOut << object->toString().GetData() << std::endl;
}

void
lcpp::Printer::print(const std::string& str)
{
    *m_pOut << str;
}

void
lcpp::Printer::print(const ezString& str)
{
    print(str.GetData());
}

void
lcpp::Printer::print(const ezStringBuilder& str)
{
    print(str.GetData());
}

void
lcpp::Printer::print(const char* str)
{
    *m_pOut << str;
}

void
lcpp::Printer::setOutputStream(std::ostream& out)
{
    m_pOut = &out;
}
