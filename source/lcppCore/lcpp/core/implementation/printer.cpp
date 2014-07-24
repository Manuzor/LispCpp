#include "stdafx.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/typeSystem.h"
#include <iostream>
#include <string>

// Enable this to allow debug messages
#define VerboseDebugMessage LCPP_LOGGING_VERBOSE_DEBUG_FUNCTION_NAME

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
    EZ_LOG_BLOCK("Printer::print", "LispObject");
    ezLog::VerboseDebugMessage("Type: %s", object->type().name);

    if(object->is<LispVoid>()){ return; }

    *m_pOut << object->toString().GetData() << std::endl;
}

void
lcpp::Printer::print(const std::string& str)
{
    EZ_LOG_BLOCK("Printer::print", "std::string");

    *m_pOut << str;
}

void
lcpp::Printer::print(const String& str)
{
    EZ_LOG_BLOCK("Printer::print", "String");

    print(str.GetData());
}

void
lcpp::Printer::print(const ezStringBuilder& str)
{
    EZ_LOG_BLOCK("Printer::print", "ezStringBuilder");

    print(str.GetData());
}

void
lcpp::Printer::print(const char* str)
{
    EZ_LOG_BLOCK("Printer::print", "const char*");

    *m_pOut << str;
}

void
lcpp::Printer::setOutputStream(std::ostream& out)
{
    EZ_LOG_BLOCK("Printer::setOutputStream", "std::ostream");

    m_pOut = &out;
}
