#pragma once

namespace lcpp
{
    class LispObjectHeader;
    class LispObject;
    class MetaInfo;
    class Type;
    class AttributeFlags;

    namespace object
    {
        template<typename T_Data>
        StackPtr<LispObject> create(Ptr<const MetaInfo> pMetaInfo);

        template<typename T_Data>
        StackPtr<LispObject> create(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo);

        template<typename T_Data>
        Ptr<LispObject> createStatic(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo);

        LCPP_API_CORE_CONT bool isType(StackPtr<LispObject> pObject, const Type& type);

        LCPP_API_CORE_CONT const Type& getType(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT const AttributeFlags& getAttributes(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT const MetaInfo& getMetaInfo(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT bool isCallable(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isBuiltin(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isNameable(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isEnvironmentContainer(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> call(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT StackPtr<LispObject> toString(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> getName(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT void setName(StackPtr<LispObject> pObject, StackPtr<LispObject> pName);
        LCPP_API_CORE_CONT bool hasName(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT StackPtr<LispObject> getEnvironment(StackPtr<LispObject> pObject);
        LCPP_API_CORE_CONT void setEnvironment(StackPtr<LispObject> pObject, StackPtr<LispObject> pEnv);
        LCPP_API_CORE_CONT bool hasEnvironment(StackPtr<LispObject> pObject);
    }

}

#include "lcpp/core/typeSystem/impl/object.inl"
