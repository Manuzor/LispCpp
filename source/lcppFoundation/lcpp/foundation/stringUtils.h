#pragma once

namespace lcpp
{
    inline bool contains(const String& str, ezUInt32 character)
    {
        auto iter = str.GetIteratorFront();

        for(auto iter = str.GetIteratorFront(); iter.IsValid(); ++iter)
        {
            if (iter.GetCharacter() == character)
            {
                return true;
            }
        }

        return false;
    }

    inline bool contains(const char* cstr, char character)
    {
        // TODO more efficient implementation?
        return contains(String(cstr), character);
    }

    inline bool isWhiteSpace(ezUInt32 character)
    {
        return ezStringUtils::IsWhiteSpace(character);
    }

    inline bool isDigit(ezUInt32 character)
    {
        return character >= '0' && character <= '9';
    }

    inline bool isAllDigits(const String& str)
    {
        isAllDigits(str.GetData());
    }

    inline bool isAllDigits(const char* cstr)
    {
        LCPP_NOT_IMPLEMENTED;
    }
}
