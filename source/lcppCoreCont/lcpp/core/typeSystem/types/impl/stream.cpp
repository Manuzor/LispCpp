#include "stdafx.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/types/streamData.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/objectData.h"

#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stringData.h"

namespace lcpp
{
    namespace stream
    {
        static void scan(CollectableBase* pCollectable, GarbageCollectionContext* pGC)
        {
            auto pStream = static_cast<LispObject*>(pCollectable);
            typeCheck(pStream, Type::Stream);

            auto& pString = pStream->getData<Data>().m_pString.get();
            pString = pGC->addSurvivor(pString);
        }

        static bool isEqual(Ptr<LispObject> pLhs, Ptr<LispObject> pRhs)
        {
            return getIterator(pLhs) == getIterator(pRhs);
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::Stream);
                meta.setPrettyName("stream");
                meta.addProperty(MetaProperty(MetaProperty::Builtin::ScanFunction,
                                              static_cast<ScanFunction_t>(&scan)));
                    meta.addProperty(MetaProperty(MetaProperty::Builtin::IsEqualFunction,
                                                  static_cast<IsEqualFunction_t>(&isEqual)));

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create(StackPtr<LispObject> pString)
        {
            LCPP_LogBlock("stream::create");
            typeCheck(pString, Type::String);

            auto pInstance = object::create<Data>(getMetaInfo());

            auto& data = pInstance->getData<Data>();

            data.m_pString = pString.get();
            auto pFirst = data.m_pString->getData<str::Data>().m_szString;
            auto pEnd = pFirst + data.m_pString->getData<str::Data>().m_uiLength;
            data.m_stringView = ezStringIterator(pFirst, pEnd, pFirst);

            return pInstance;
        }

        ezStringIterator& getIterator(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->getData<Data>().m_stringView;
        }

        Ptr<LispObject> getString(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->getData<Data>().m_pString;
        }

        ezUInt32 getCharacter(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            return iterator.GetCharacter();
        }

        bool isValid(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            return iterator.IsValid();
        }

        ezUInt32 next(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;
            ++iterator;
            return iterator.GetCharacter();
        }

        ezUInt32 getPosition(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->getData<Data>().m_stringView;

            if(!iterator.IsValid())
            {
                return EndOfStream;
            }

            auto position = ptrdiff_t(iterator.GetData() - iterator.GetStart());
            return ezUInt32(position);
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pObject)
        {
            ezStringBuilder theString;
            theString.Append("<stream at ");

            if (!isValid(pObject))
            {
                theString.Append("end");
            }
            else
            {
                auto position = getPosition(pObject);
                auto currentData = getIterator(pObject).GetData();

                theString.AppendFormat("%u: \"%s\"", position, currentData);
            }

            theString.Append('>');
            return str::create(theString.GetData(), theString.GetElementCount());
        }

    }
}
