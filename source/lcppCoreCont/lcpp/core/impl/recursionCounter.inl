#include "lcpp/core/runtime.h"

namespace lcpp
{

    EZ_FORCE_INLINE
    RecursionCounter::RecursionCounter(Ptr<LispRuntimeState> pRunTimeState) :
        m_pRunTimeState(pRunTimeState)
    {
        m_pRunTimeState->increaseRecursionDepth();
    }

    EZ_FORCE_INLINE
    RecursionCounter::~RecursionCounter()
    {
        m_pRunTimeState->decreaseRecursionDepth();
    }
}
