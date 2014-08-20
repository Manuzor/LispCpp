#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/objectData.h"

#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    namespace object
    {
        bool isType(Ptr<LispObject> pObject, const Type& type)
        {
            EZ_ASSERT(pObject, "Invalid object.");
            return getType(pObject) == type;
        }

        const Type& getType(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid object.");
            EZ_ASSERT(pObject->m_header.m_pMetaInfo, "No valid type info.");
            return pObject->m_header.m_pMetaInfo->getType();
        }

        const AttributeFlags& getAttributes(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid object.");
            EZ_ASSERT(pObject->m_header.m_pMetaInfo, "No valid type info.");
            return pObject->m_header.m_pMetaInfo->getAttributes();
        }

        const MetaInfo& getMetaInfo(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid object.");
            EZ_ASSERT(pObject->m_header.m_pMetaInfo, "No valid type info.");
            return *pObject->m_header.m_pMetaInfo;
        }

        const LispObjectHeader& getHeader(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid object.");
            return pObject->m_header;
        }

        bool isCallable(Ptr<LispObject> pObject)
        {
            return getAttributes(pObject).isCallable();
        }

        bool isBuiltin(Ptr<LispObject> pObject)
        {
            return getAttributes(pObject).isBuiltin();
        }

        bool isNameable(Ptr<LispObject> pObject)
        {
            return getAttributes(pObject).isNameable();
        }

        bool isEnvironmentContainer(Ptr<LispObject> pObject)
        {
            return getAttributes(pObject).isEnvironmentContainer();
        }

        Ptr<LispObject> call(Ptr<LispObject> pCont)
        {
            typeCheck(pCont, Type::Continuation);

            auto pStack = cont::getStack(pCont);

            auto pCallable = pStack->get(-1);

            if(!isCallable(pCallable))
            {
                LCPP_THROW(exceptions::InvalidInput("Argument is not callable."));
            }

            auto& type = object::getType(pCallable);

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
                LCPP_cont_tailCall(pCont, &lambda::userDefined::call);
            }

            EZ_REPORT_FAILURE("pCallable has the Callable attribute set "
                              "but is not supported in the switch above. "
                              "Did you forget to add code to the switch?");

            return nullptr;
        }

        Ptr<LispObject> toString(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

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
            case Type::Lambda: return isBuiltin(pObject) ? lambda::builtin::toString(pObject) : lambda::userDefined::toString(pObject);
            case Type::Syntax: return isBuiltin(pObject) ? syntax::builtin::toString(pObject) : throw exceptions::NotImplemented();
            case Type::Environment: return env::toString(pObject);

            case Type::File: return file::toString(pObject);

            case Type::Continuation: return cont::toString(pObject);
            case Type::Time: return time::toString(pObject);
            }

            EZ_REPORT_FAILURE("Unsupported type for toString.");
            LCPP_THROW(exceptions::InvalidInput("Unsupported type for toString."));
        }

        Ptr<LispObject> getName(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            switch(type.getId())
            {
            case Type::Lambda: return isBuiltin(pObject) ? lambda::builtin::getName(pObject) : lambda::userDefined::getName(pObject);
            case Type::Syntax: return isBuiltin(pObject) ? syntax::builtin::getName(pObject) : throw exceptions::NotImplemented();
            }

            EZ_REPORT_FAILURE("Unsupported type for getName.");
            LCPP_THROW(exceptions::InvalidInput("Unsupported type for getName."));
        }

        void setName(Ptr<LispObject> pObject, Ptr<LispObject> pName)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            switch(type.getId())
            {
            case Type::Lambda: isBuiltin(pObject) ? lambda::builtin::setName(pObject, pName) : lambda::userDefined::setName(pObject, pName); return;
            case Type::Syntax: isBuiltin(pObject) ? syntax::builtin::setName(pObject, pName) : throw exceptions::NotImplemented(); return;
            }

            EZ_REPORT_FAILURE("Unsupported type for getName.");
            LCPP_THROW(exceptions::InvalidInput("Unsupported type for getName."));
        }

        bool hasName(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            switch(type.getId())
            {
            case Type::Lambda: return isBuiltin(pObject) ? lambda::builtin::hasName(pObject) : lambda::userDefined::hasName(pObject);
            case Type::Syntax: return isBuiltin(pObject) ? syntax::builtin::hasName(pObject) : throw exceptions::NotImplemented();
            }

            EZ_REPORT_FAILURE("Unsupported type for getName.");
            LCPP_THROW(exceptions::InvalidInput("Unsupported type for getName."));
        }

        Ptr<LispObject> getEnvironment(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            switch(type.getId())
            {
            case Type::Lambda: return isBuiltin(pObject) ? lambda::builtin::getEnvironment(pObject) : lambda::userDefined::getEnvironment(pObject);
            }

            auto message = ezStringBuilder();
            message.Format("Unsupported type for getEnvironment.");

            EZ_REPORT_FAILURE(message.GetData());
            LCPP_THROW(exceptions::InvalidInput(message.GetData()));
        }

        void setEnvironment(Ptr<LispObject> pObject, Ptr<LispObject> pEnv)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            // TODO Support types for this operation?

            auto message = ezStringBuilder();
            message.Format("Unsupported type for setEnvironment.");

            EZ_REPORT_FAILURE(message.GetData());
            LCPP_THROW(exceptions::InvalidInput(message.GetData()));
        }

        bool hasEnvironment(Ptr<LispObject> pObject)
        {
            EZ_ASSERT(pObject, "Invalid pointer.");

            auto& type = object::getType(pObject);

            switch(type.getId())
            {
            case Type::Lambda: return true;
            }

            auto message = ezStringBuilder();
            message.Format("Unsupported type for hasEnvironment.");

            EZ_REPORT_FAILURE(message.GetData());
            LCPP_THROW(exceptions::InvalidInput(message.GetData()));
        }
    }
}