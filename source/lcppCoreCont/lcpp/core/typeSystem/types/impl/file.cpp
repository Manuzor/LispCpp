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
                message.AppendFormat("Unable to open file '%s'. ", str::getValue(pStringFileName).GetData());
                message.AppendFormat("Supported file modes are one of \"r\"ead, \"w\"write, and \"a\"ppend, got \"%s\".", fileModeName.GetData());
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

        Ptr<LispObject> getFileName(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);
            return pFile->m_file.getFileName();
        }

        void setFileName(Ptr<LispObject> pFile, Ptr<LispObject> pFileName)
        {
            typeCheck(pFile, Type::File);
            pFile->m_file.setFileName(pFileName);
        }

        Ptr<LispObject> toString(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto output = ezStringBuilder();

            auto& wrappedFile = pFile->m_file.getFile();
            auto pFileName = pFile->m_file.getFileName();

            if (wrappedFile.IsOpen())
            {
                auto& fileNameValue = str::getValue(pFileName);
                output.AppendFormat("<open %s: \"%s\">", metaInfo().getPrettyName(), fileNameValue.GetData());
            }
            else
            {
                if(isNil(pFileName))
                {
                    output.AppendFormat("<closed %s>", metaInfo().getPrettyName());
                }
                else
                {
                    auto& fileNameValue = str::getValue(pFileName);
                    output.AppendFormat("<closed %s: \"%s\">", metaInfo().getPrettyName(), fileNameValue.GetData());
                }
            }

            return str::create(output.GetData());
        }

    }
}
