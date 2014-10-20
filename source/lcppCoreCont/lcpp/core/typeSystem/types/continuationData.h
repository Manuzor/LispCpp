#pragma once
#include "lcpp/core/containers/stack.h"
#include "lcpp/core/typeSystem/types/continuationCommon.h"

namespace lcpp
{
    class LispObject;
    class LispRuntimeState;

    namespace cont
    {
        class Data
        {
        public:
            Data() :
                m_stack(*new Stack())
            {
            }

            ~Data()
            {
                delete &m_stack;
            }

            Data(const Data&) = delete;
            void operator =(const Data&) = delete;

            Ptr<LispRuntimeState> m_pRuntimeState;
            Ptr<LispObject> m_pParent;
            Function_t m_pFunction;
            Stack& m_stack;
            UserData_t m_userData;
        };
    }
}
