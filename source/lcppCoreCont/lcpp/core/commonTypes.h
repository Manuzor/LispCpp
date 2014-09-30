#pragma once

namespace lcpp
{
    class LispObject;

    namespace number
    {
        typedef ezInt64 Integer_t;
        typedef double Float_t;
        typedef float FloatAlternative_t;
    }

    namespace symbol
    {
        typedef ezHybridString<32, AllocatorWrapper_Default> String;
    }

    namespace str
    {
        typedef ezHybridString<64, AllocatorWrapper_Default> String;
    }

    namespace env
    {
        typedef ezHashTable<StackPtr<LispObject>, StackPtr<LispObject>, ezHashHelper<StackPtr<LispObject>>, AllocatorWrapper_Default> HashTable;
    }
}
