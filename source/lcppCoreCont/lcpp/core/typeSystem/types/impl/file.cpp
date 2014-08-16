#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/objectData.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/metaInfo.h"

#include "lcpp/core/exceptions/fileException.h"

#include "lcpp/core/typeSystem/types/file.h"
#include "lcpp/core/typeSystem/types/fileData.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/bool.h"

namespace lcpp
{
    namespace file
    {
        const MetaInfo& metaInfo()
        {
            static auto meta = MetaInfo(Type::File, "file");
            return meta;
        }

        Ptr<LispObject> create()
        {
            auto pInstance = object::create<Data>(metaInfo());

            auto& data = pInstance->m_file;

            new (data.m_pFileName) Ptr<LispObject>(LCPP_pNil);
            new (data.m_file) ezOSFile();

            return pInstance;
        }

        Ptr<LispObject> open(Ptr<LispObject> pFile, Ptr<LispObject> pStringFileName, Ptr<LispObject> pStringFileMode)
        {
            typeCheck(pFile, Type::File);

            pFile->m_file.setFileName(pStringFileName);
            auto& wrappedFile = pFile->m_file.getFile();
            if(wrappedFile.IsOpen())
            {
                wrappedFile.Close();
            }
            auto openMode = ezFileMode::None;
            auto& fileModeName = str::getValue(pStringFileMode);

            if (fileModeName.IsEqual("r"))
            {
                openMode = ezFileMode::Read;
            }
            else if(fileModeName.IsEqual("w"))
            {
                openMode = ezFileMode::Write;
            }
            else if(fileModeName.IsEqual("a"))
            {
                openMode = ezFileMode::Append;
            }
            else
            {
                auto message = ezStringBuilder();
                message.Format("Supported file modes are one of \"r\"ead, \"w\"write, and \"a\"ppend, got %s", fileModeName.GetData());
                LCPP_THROW(exceptions::InvalidFileMode(message));
            }

            auto result = wrappedFile.Open(str::getValue(pStringFileName).GetData(), openMode);

            return result.Succeeded() ? LCPP_pTrue : LCPP_pFalse;
        }

        Ptr<LispObject> isOpen(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto& wrappedFile = pFile->m_file.getFile();

            return wrappedFile.IsOpen() ? LCPP_pTrue : LCPP_pFalse;
        }

        void close(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto& wrappedFile = pFile->m_file.getFile();
            wrappedFile.Close();

        }

        Ptr<LispObject> toString(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto output = ezStringBuilder();
            output.AppendFormat("<%s ", metaInfo().getPrettyName());

            auto& wrappedFile = pFile->m_file.getFile();

            if (wrappedFile.IsOpen())
            {
                auto& fileNameValue = str::getValue(pFile->m_file.getFileName());
                output.AppendFormat("(open): %s", metaInfo().getPrettyName(), fileNameValue.GetData());
            }
            else
            {
                output.Append("(closed)");
            }
            output.Append('>');

            return str::create(output.GetData());
        }

    }
}
