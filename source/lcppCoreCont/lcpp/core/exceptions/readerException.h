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

        class MissingLeftListDelimiter :
            public ReaderBase
        {
        public:
            MissingLeftListDelimiter(const String& message) :
                ReaderBase(message)
            {
            }
        };

        class MissingRightListDelimiter :
            public ReaderBase
        {
        public:
            MissingRightListDelimiter(const String& message) :
                ReaderBase(message)
            {
            }
        };
    }
}
