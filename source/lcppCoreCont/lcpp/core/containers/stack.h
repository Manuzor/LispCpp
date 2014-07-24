#pragma once

namespace lcpp
{
    class Stack :
        public StackBase<Ptr<ezUInt32>, 32>
    {
    public:

        Stack();
    };
    
}
