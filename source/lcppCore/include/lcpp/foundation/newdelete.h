#pragma once

#include <new>

#ifdef new
#undef new
#endif // new

#ifdef delete
#undef delete
#endif // delete

void* operator new      (size_t size);
void* operator new[]    (size_t size);
void  operator delete   (void* mem);
void  operator delete[] (void* mem);
