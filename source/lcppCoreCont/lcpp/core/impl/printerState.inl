#pragma once

namespace lcpp
{
    namespace printer
    {
        EZ_FORCE_INLINE
        State::State() :
            m_pOutStream(&m_stdOutStream)
        {
        }

        EZ_FORCE_INLINE
        State::~State()
        {
            m_pOutStream = nullptr;
        }

        EZ_FORCE_INLINE
        void State::reset()
        {
            m_pOutStream = &m_stdOutStream;
        }
    }
}
