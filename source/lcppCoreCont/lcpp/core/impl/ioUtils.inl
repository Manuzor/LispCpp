#include "lcpp/core/typeSystem/objectUtils.h"

namespace lcpp
{
    EZ_FORCE_INLINE
    StandardOutputStream::StandardOutputStream()
    {
    }

    EZ_FORCE_INLINE
    ezResult StandardOutputStream::WriteBytes(const void* pWriteBuffer, ezUInt64 uiBytesToWrite)
    {
        std::cout.write((const char*)pWriteBuffer, uiBytesToWrite);
        return EZ_SUCCESS;
    }

    EZ_FORCE_INLINE
    ezResult StandardOutputStream::Flush()
    {
        std::cout.flush();
        return EZ_SUCCESS;
    }

    // Operator overloads.
    //////////////////////////////////////////////////////////////////////////
    
    inline
    ezStreamWriterBase& operator << (ezStreamWriterBase& stream, Ptr<LispObject> pObject)
    {
        auto pStringObject = toString(pObject);
        
        const auto& stringValue = str::getValue(pStringObject);

        stream.WriteBytes(stringValue.GetData(), stringValue.GetElementCount());

        return stream;
    }
}


