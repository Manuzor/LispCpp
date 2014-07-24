#pragma once

namespace lcpp
{
    class LispObject;

    class LCPP_API_CORE LispObjectStack :
        public StackBase<Ptr<LispObject>, 64>
    {
    public:

        static Ptr<LispObjectStack> create();

    protected:

        LispObjectStack(Ptr<ezAllocatorBase> pAllocator);

    };
}

#include "lcpp/core/containers/implementation/objectStack.inl"
