#pragma once

namespace lcpp
{
    class LispObject;

    namespace lambda
    {
        namespace userDefined
        {
            class Data
            {
            public:
                Ptr<LispObject> m_pName;
                Ptr<LispObject> m_pEnv;
                Ptr<LispObject> m_pArgList;
                Ptr<LispObject> m_pBody;
            };
        }
    }
}
