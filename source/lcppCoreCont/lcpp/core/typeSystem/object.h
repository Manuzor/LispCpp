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
        Ptr<LispObject> create(Ptr<const MetaInfo> pMetaInfo);

        template<typename T_Data>
        Ptr<LispObject> create(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo);

        template<typename T_Data>
        Ptr<LispObject> createStatic(Ptr<GarbageCollector> pGarbageCollector, Ptr<const MetaInfo> pMetaInfo);

        LCPP_API_CORE_CONT bool isType(Ptr<LispObject> pObject, const Type& type);

        LCPP_API_CORE_CONT const Type& getType(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT const AttributeFlags& getAttributes(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT const MetaInfo& getMetaInfo(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT bool isCallable(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isBuiltin(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isNameable(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isEnvironmentContainer(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> call(StackPtr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(StackPtr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> isEqual(StackPtr<LispObject> pLhs, StackPtr<LispObject> pRhs);

        LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT void setName(Ptr<LispObject> pObject, Ptr<LispObject> pName);
        LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT void setEnvironment(Ptr<LispObject> pObject, Ptr<LispObject> pEnv);
        LCPP_API_CORE_CONT bool hasEnvironment(Ptr<LispObject> pObject);
    }

}

#include "lcpp/core/typeSystem/impl/object.inl"
