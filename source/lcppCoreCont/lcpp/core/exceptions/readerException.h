#pragma once

namespace lcpp
{
    namespace exceptions
    {
        class Reader :
            public ExceptionBase
        {
        public:
            Reader(const String& message) :
                ExceptionBase(message)
            {
            }
        };

        class MissingStringDelimiter :
            public Reader
        {
        public:
            MissingStringDelimiter(const String& message) :
                Reader(message)
            {
            }
        };

        class MissingListDelimiter :
            public Reader
        {
        public:
            MissingListDelimiter(const String& message) :
                Reader(message)
            {
            }
        };
    }
}
