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
        typedef ezHashTable<LispObject*, LispObject*, ezHashHelper<LispObject*>, AllocatorWrapper_Default> HashTable;
    }

    typedef bool(*IsEqualFunction_t)(Ptr<LispObject>, Ptr<LispObject>);
}
