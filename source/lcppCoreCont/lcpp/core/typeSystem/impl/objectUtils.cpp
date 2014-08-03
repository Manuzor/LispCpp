#include "stdafx.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectUtils.h"

#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    Ptr<LispObject> call(Ptr<LispObject> pCont)
    {
        typeCheck(pCont, Type::Continuation);

        auto pStack = cont::getStack(pCont);

        auto pCallable = pStack->get(-1);

        if(!isCallable(pCallable))
        {
            LCPP_THROW(exceptions::InvalidInput("Argument is not callable."));
        }

        auto& type = pCallable->getType();

        switch(type.getId())
        {
        case Type::Syntax:
            if(isBuiltin(pCallable))
            {
                LCPP_cont_tailCall(pCont, &syntax::builtin::call);
            }
            // TODO support user defined syntax.
            LCPP_NOT_IMPLEMENTED;
        case Type::Lambda:
            if (isBuiltin(pCallable))
            {
                LCPP_cont_tailCall(pCont, &lambda::builtin::call);
            }
            // TODO support user defined lambdas.
            LCPP_NOT_IMPLEMENTED;
        }

        EZ_REPORT_FAILURE("pCallable has the Callable attribute set "
                          "but is not supported in the switch above. "
                          "Did you forget to add code to the switch?");

        return nullptr;
    }

    String toString(Ptr<LispObject> pObject)
    {
        EZ_ASSERT(pObject, "Invalid pointer.");

        auto& type = pObject->getType();

        switch(type.getId())
        {
        case Type::Nil: return nil::toString(pObject);
        case Type::Void: return void_::toString(pObject);
        case Type::True: return true_::toString(pObject);
        case Type::False: return false_::toString(pObject);

        case Type::Integer:
        case Type::Float: return number::toString(pObject);

        case Type::Symbol: return symbol::toString(pObject);
        case Type::String: return str::toString(pObject);
        case Type::Stream: return stream::toString(pObject);

        case Type::Cons: return cons::toString(pObject);
        case Type::Lambda: return isBuiltin(pObject) ? lambda::builtin::toString(pObject) : "Not Implemented";
        case Type::Syntax: return isBuiltin(pObject) ? syntax::builtin::toString(pObject) : "Not implemented.";
        case Type::Environment: return env::toString(pObject);

        case Type::File: return "Not implemented.";

        case Type::Continuation: return cont::toString(pObject);
        }

        EZ_REPORT_FAILURE("Unsupported type for toString.");
        LCPP_THROW(exceptions::InvalidInput("Unsupported type for toString."));
    }

}
