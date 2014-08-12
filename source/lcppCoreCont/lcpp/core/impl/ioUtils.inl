#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/sourceCursor.h"

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
    ezStreamWriterBase& operator << (ezStreamWriterBase& stream, const SourceCursor& cursor)
    {
        auto& position = cursor.getPosition();

        auto output = ezStringBuilder();
        output.Format("%u(%u)", position.m_line, position.m_column);
        stream << output.GetData();
        
        return stream;
    }
}


