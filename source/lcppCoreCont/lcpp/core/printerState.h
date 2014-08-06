#pragma once

namespace lcpp
{
    namespace printer
    {
        class State
        {
        public:

            Ptr<ezStreamWriterBase> m_pOutStream;

        public:

            State();
        };
    }
}

#include "lcpp/core/impl/printerState.inl"
