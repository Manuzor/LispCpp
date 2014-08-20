#pragma once

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/typeSystem/includeAllTypeDatas.h"

namespace lcpp
{
    class LispObjectHeader
    {
    public:

        Ptr<const MetaInfo> m_pMetaInfo;

    };

    class LCPP_API_CORE_CONT LispObject
    {
    public:

        LispObjectHeader m_header;

        union
        {
            number::Integer_t m_integer;
            number::Float_t m_float;
            symbol::Data m_symbol;
            str::Data m_string;
            env::Data m_env;
            cont::Data m_cont;
            stream::Data m_stream;
            cons::Data m_cons;
            lambda::builtin::Data m_lambda_builtin;
            lambda::userDefined::Data m_lambda_userDefined;
            syntax::builtin::Data m_syntax_builtin;
            time::Data m_time;
            file::Data m_file;
        };

    public:

        LispObject(const MetaInfo& metaInfo);

    };
}

#include "lcpp/core/typeSystem/impl/objectData.inl"
