#pragma once

namespace lcpp
{
    namespace file
    {
        class Data
        {
        public:
            ezOSFile m_file;
            Ptr<LispObject> m_pFileName;
        };
    }
}
