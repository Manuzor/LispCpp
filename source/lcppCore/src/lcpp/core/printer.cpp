#include "stdafx.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/typeSystem.h"
#include <iostream>

void lcpp::Printer::print(SchemeObject& object)
{
    std::cout << object.toString().GetData() << std::endl;
}
