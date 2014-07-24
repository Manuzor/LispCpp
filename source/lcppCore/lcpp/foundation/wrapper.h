#pragma once

namespace lcpp
{
    typedef ezHybridString<32, AllocatorWrapper_Default> String;

    // TODO C++11...

    //template<typename T>
    //using StringBuilder = ezStringBuilder<T, AllocatorWrapper_Default>;
    //
    //template<typename T>
    //using String = String<T, AllocatorWrapper_Default>;
    //
    //template<typename T>
    //using DynamicArray = ezDynamicArray<T, AllocatorWrapper_Default>;
    //
    //template<typename T, ezUInt32 N_Size>
    //using HybridArray = ezHybridArray<T, N_Size, AllocatorWrapper_Default>;

}
