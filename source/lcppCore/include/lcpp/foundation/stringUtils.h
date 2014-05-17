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
}
