#pragma once

namespace lcpp
{
    class LispObject;

    class LCPP_API_CORE_CONT Stack :
        public StackBase<Ptr<LispObject>, 32>
    {
    public:

        Stack();
    };
    
}
