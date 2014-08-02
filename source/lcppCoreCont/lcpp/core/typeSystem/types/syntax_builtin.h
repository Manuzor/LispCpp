#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace syntax
    {
        namespace builtin
        {

            typedef Ptr<LispObject>(*Function_t)(Ptr<LispObject>);

            LCPP_API_CORE_CONT const MetaInfo& metaInfo();

            LCPP_API_CORE_CONT Ptr<LispObject> create(Function_t pFunction);

            //////////////////////////////////////////////////////////////////////////

            class Data
            {
            public:

                Function_t getFunction();

            public:

                Function_t m_pFunction;

            };

            //////////////////////////////////////////////////////////////////////////

            LCPP_API_CORE_CONT Ptr<LispObject> call(Ptr<LispObject> pCont);

            LCPP_API_CORE_CONT Function_t getFunction(Ptr<LispObject> pSyntax);
        }
    }
}

#include "lcpp/core/typeSystem/types/impl/syntax_builtin.inl"
