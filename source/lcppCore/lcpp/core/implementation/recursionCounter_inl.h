#include "lcpp/core/runtime.h"

inline
lcpp::RecursionCounter::RecursionCounter(Ptr<LispRuntime> pRunTime) :
    pRunTime(pRunTime)
{
    pRunTime->increaseRecursionDepth();
}

inline
lcpp::RecursionCounter::~RecursionCounter()
{
    pRunTime->decreaseRecursionDepth();
}
