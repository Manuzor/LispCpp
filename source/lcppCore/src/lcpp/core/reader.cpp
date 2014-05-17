#include "stdafx.h"
#include "lcpp/core/reader.h"
#include "lcpp/foundation/conversion.h"
#include "lcpp/exceptions/exceptions.h"
#include "lcpp/core/typeSystem.h"


lcpp::Reader::Reader() :
    m_separators(CInfo().separators),
    m_factoryDefault(),
    m_pFactory(&m_factoryDefault)
{
}

lcpp::Reader::Reader(const CInfo& cinfo) :
    m_separators(cinfo.separators),
    m_factoryDefault(),
    m_pFactory(cinfo.pFactory ? cinfo.pFactory : &m_factoryDefault)
{
}

lcpp::Reader::~Reader()
{
}

lcpp::SchemeObject* lcpp::Reader::read(const ezString& input)
{
    auto iter = input.GetIteratorFront();
    return read(iter);
}

lcpp::SchemeObject* lcpp::Reader::read(ezStringIterator& input)
{
    SchemeObject* pResultObject = nullptr;

    skipSeparators(input);

    if(!input.IsValid())
    {
        throw exceptions::InvalidInput("Input string only contained whitespace!");
    }

    auto ch = input.GetCharacter();

    switch(ch)
    {
    case ')':
        throw exceptions::InvalidInput("Unexpected character ')'.");
    case '\'':
        // TODO read quote
        break;
    case '"':
        // TODO read string
        break;
    case '(':
        // TODO read list
        break;
    default:
        // Try parsing for an integer first, then a symbol
        {
            const char* lastPos = nullptr;
            SchemeInteger::Number_t integer;
            auto result = to(input, integer, &lastPos);

            // The string contains a number, but it is a floating point number; reparse.
            if (result.IsSuccess())
            {
                LCPP_SCOPE_EXIT{ input.SetCurrentPosition(lastPos); };

                if(lastPos[0] == '.')
                {
                    SchemeNumber::Number_t number;
                    result = to(input, number, &lastPos);
                    EZ_ASSERT(result.IsSuccess(), "An integer of the form '123.' should be parsed as float!");
                    pResultObject = m_pFactory->createNumber(number);
                    break;
                }
                
                pResultObject = m_pFactory->createInteger(integer);
                break;
            }
            // TODO parse symbol
        }
        break;
    }

    return pResultObject;
}

void lcpp::Reader::skipSeparators(ezStringIterator& iter)
{
    while(iter.IsValid() && isSeparator(iter.GetCharacter()))
    {
        ++iter;
    }
}

bool lcpp::Reader::isSeparator(ezUInt32 character)
{
    for(auto iter = m_separators.GetIteratorFront(); iter.IsValid(); ++iter)
    {
        if(character == iter.GetCharacter())
        {
            return true;
        }
    }

    return false;
}
