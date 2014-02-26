#pragma once

// declare in global namespace for C++11 range-based for loops

template<typename T, typename Derived>
T* begin(ezArrayBase<T, Derived>& arr);

template<typename T, typename Derived>
const T* begin(const ezArrayBase<T, Derived>& arr);

template<typename T, typename Derived>
T* end(ezArrayBase<T, Derived>& arr);

template<typename T, typename Derived>
const T* end(const ezArrayBase<T, Derived>& arr);

#include "lcpp/foundation/implementation/rangeUtils_inl.h"
