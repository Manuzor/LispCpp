#pragma once

namespace lcpp
{
    namespace exceptions
    {
        class ReaderBase :
            public ExceptionBase
        {
        public:
            ReaderBase(const String& message) :
                ExceptionBase(message)
            {
            }
        };

        class MissingStringDelimiter :
            public ReaderBase
        {
        public:
            MissingStringDelimiter(const String& message) :
                ReaderBase(message)
            {
            }
        };

        class MissingListDelimiter :
            public ReaderBase
        {
        public:
            MissingListDelimiter(const String& message) :
                ReaderBase(message)
            {
            }
        };
    }
}
