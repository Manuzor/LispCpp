#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;
    class Stack;

    namespace cons
    {
        class Data
        {
        public:
            Ptr<LispObject> m_pCar;
            Ptr<LispObject> m_pCdr;
        };
    }
}
