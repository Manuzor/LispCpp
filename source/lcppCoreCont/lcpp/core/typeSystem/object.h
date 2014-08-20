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
        Ptr<LispObject> create(const MetaInfo& metaInfo);

        LCPP_API_CORE_CONT bool isType(Ptr<LispObject> pObject, const Type& type);

        LCPP_API_CORE_CONT const Type& getType(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT const AttributeFlags& getAttributes(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT const MetaInfo& getMetaInfo(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT const LispObjectHeader& getHeader(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT bool isCallable(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isBuiltin(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isNameable(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT bool isEnvironmentContainer(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

        LCPP_API_CORE_CONT Ptr<LispObject> toString(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> getName(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT void setName(Ptr<LispObject> pObject, Ptr<LispObject> pName);
        LCPP_API_CORE_CONT bool hasName(Ptr<LispObject> pObject);

        LCPP_API_CORE_CONT Ptr<LispObject> getEnvironment(Ptr<LispObject> pObject);
        LCPP_API_CORE_CONT void setEnvironment(Ptr<LispObject> pObject, Ptr<LispObject> pEnv);
        LCPP_API_CORE_CONT bool hasEnvironment(Ptr<LispObject> pObject);
    }

}

#include "lcpp/core/typeSystem/impl/object.inl"