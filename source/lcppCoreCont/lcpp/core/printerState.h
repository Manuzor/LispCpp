#pragma once
#include "lcpp/core/ioUtils.h"

namespace lcpp
{
    namespace printer
    {
        class State
        {
        public:
            StandardOutputStream m_stdOutStream;
            ezStreamWriterBase* m_pOutStream;

        public:
            State();
            ~State();

            void reset();
        };
}
}

#include "lcpp/core/impl/printerState.inl"
