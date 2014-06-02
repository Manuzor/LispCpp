#pragma once

namespace lcpp
{
    inline bool contains(const ezString& str, ezUInt32 character)
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
        ezStringBuilder b;
        return contains(ezString(cstr), character);
    }

    inline bool isDigit(ezUInt32 character)
    {
        return character >= '0' && character <= '9';
    }

    inline bool isAllDigits(const ezString& str)
    {
        isAllDigits(str.GetData());
    }

    inline bool isAllDigits(const char* cstr)
    {
        EZ_ASSERT(false, "Not implemented.");
        return false;
    }
}
