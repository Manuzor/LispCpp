#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stringData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/objectData.h"

namespace lcpp
{
    namespace str
    {
        static void destroy(CollectableBase* pCollectable)
        {
            auto pString = static_cast<LispObject*>(pCollectable);
            typeCheck(pString, Type::String);

            defaultAllocator()->Deallocate(pString->getData<Data>().m_szString);
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::String);
                meta.setPrettyName("string");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::DestructorFunction,
                                              static_cast<DestructorFunction_t>(&destroy)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(const char* szStringData, ezUInt64 uiLen)
        {
            LCPP_LogBlock("str::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            if(szStringData != nullptr)
            {
                auto szString = (char*)defaultAllocator()->Allocate(uiLen + 1, EZ_ALIGNMENT_OF(char*));
                ezMemoryUtils::Copy(szString, szStringData, uiLen + 1);
                szString[uiLen] = '\0';
                pInstance->getData<Data>().m_szString = szString;
            }
            else
            {
                pInstance->getData<Data>().m_szString = nullptr;
            }

            pInstance->getData<Data>().m_uiLength = uiLen;
            return pInstance;
        }

        String getValue(Ptr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            return pObject->getData<Data>().m_szString;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            typeCheck(pObject, Type::String);

            auto uiOriginalLen = pObject->getData<Data>().m_uiLength;
            auto szOriginalData = pObject->getData<Data>().m_szString;
            auto uiLen = uiOriginalLen + 3;
            auto szData = (char*)alloca(uiOriginalLen + 3);

            // String layout:               "...."\0
            szData[0] = '"';          // ---^
            szData[uiLen - 2] = '"';  // --------^
            szData[uiLen - 1] = '\0'; // ---------^^
            ezMemoryUtils::Copy(szData + 1, szOriginalData, uiOriginalLen);

            return str::create(szData, uiLen);
        }

    }
}
