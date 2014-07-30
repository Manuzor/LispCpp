#include "stdafx.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

namespace lcpp
{
    namespace stream
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::Stream, "stream");
            return meta;
        }

        Ptr<LispObject> create(ezStringIterator& iterator)
        {
            auto pInstance = LispObject::create<Data>(metaInfo());

            auto& data = pInstance->m_stream;

            new (data.m_iterator) ezStringIterator(iterator);

            return pInstance;
        }

        ezStringIterator& getIterator(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            return pStream->m_stream.getIterator();
        }

        ezUInt32 getCharacter(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            return iterator.GetCharacter();
        }

        bool isValid(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            return iterator.IsValid();
        }

        ezUInt32 next(Ptr<LispObject> pStream)
        {
            typeCheck(pStream, Type::Stream);

            auto& iterator = pStream->m_stream.getIterator();
            ++iterator;
            return iterator.GetCharacter();
        }

    }
}
