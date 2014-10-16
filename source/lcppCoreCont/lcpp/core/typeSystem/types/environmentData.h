#pragma once

namespace lcpp
{
    class LispObject;

    namespace env
    {
        class Data
        {
        public:
            Ptr<LispObject> m_pName;
            Ptr<LispObject> m_pParent;
            HashTable m_table;
        };
    };
}
