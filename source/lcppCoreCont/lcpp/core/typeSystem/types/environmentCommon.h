#pragma once

namespace lcpp
{
    namespace env
    {
        class BindingLocation
        {
        public:

            enum Enum
            {
                None,
                Local,
                Parent,
            };

            BindingLocation(Enum value);

            bool doesNotExist() const;
            bool locally() const;
            bool inParent() const;

            operator bool() const;

        private:

            Enum m_value;
        };
    };
}
#include "lcpp/core/typeSystem/types/impl/environmentCommon.inl"
