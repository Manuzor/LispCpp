#pragma once

namespace lcpp
{
    namespace file
    {
        class Data
        {
        public:
            Data() : m_file(*new ezOSFile()) {}
            ~Data() { delete &m_file; }

            ezOSFile& m_file;
            Ptr<LispObject> m_pFileName;
        };
    }
}
