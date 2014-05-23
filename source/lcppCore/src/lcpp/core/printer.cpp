#include "stdafx.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/typeSystem.h"
#include <iostream>

lcpp::Printer::Printer() :
    m_out(&std::cout)
{
}

lcpp::Printer::~Printer()
{

}

void lcpp::Printer::print(SchemeObject& object)
{
    if(object.is<SchemeVoid>())
    {
        return;
    }

    *m_out << object.toString().GetData() << std::endl;
}

void lcpp::Printer::setOutputStream(std::ostream& out)
{
    m_out = &out;
}
