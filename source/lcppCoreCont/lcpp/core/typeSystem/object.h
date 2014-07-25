#pragma once

namespace lcpp
{

    class LispObject
    {
    public:

        bool isType(const Type& type);

    protected:

        Ptr<TypeInfo> m_pTypeInfo;
    };
}
