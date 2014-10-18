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
#include "lcpp/core/typeSystem/types/string.h"

namespace lcpp
{
    namespace file
    {
        static void destroy(CollectableBase* pCollectable)
        {
            auto pObject = static_cast<LispObject*>(pCollectable);
            typeCheck(pObject, Type::File);

            auto& theFile = pObject->getData<Data>().m_file;
            theFile.~ezOSFile();
        }

        Ptr<const MetaInfo> getMetaInfo()
        {
            static auto meta = []
            {
                auto meta = MetaInfo();
                meta.setType(Type::File);
                meta.setPrettyName("file");

                return meta;
            }(); // Note that this lambda is immediately called.

            return &meta;
        }

        Ptr<LispObject> create()
        {
            LCPP_LogBlock("file::create");

            auto pInstance = object::create<Data>(getMetaInfo());

            auto& data = pInstance->getData<Data>();

            data.m_pFileName = LCPP_pNil;

            return pInstance;
        }

        Ptr<LispObject> open(Ptr<LispObject> pFile, Ptr<LispObject> pStringFileName, Ptr<LispObject> pStringFileMode)
        {
            typeCheck(pFile, Type::File);

            pFile->getData<Data>().m_pFileName = pStringFileName;
            auto& wrappedFile = pFile->getData<Data>().m_file;
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

            auto& wrappedFile = pFile->getData<Data>().m_file;

            return wrappedFile.IsOpen() ? LCPP_pTrue : LCPP_pFalse;
        }

        void close(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto& wrappedFile = pFile->getData<Data>().m_file;
            wrappedFile.Close();

        }

        Ptr<LispObject> getFileName(Ptr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);
            return pFile->getData<Data>().m_pFileName;
        }

        void setFileName(Ptr<LispObject> pFile, Ptr<LispObject> pFileName)
        {
            typeCheck(pFile, Type::File);
            pFile->getData<Data>().m_pFileName = pFileName;
        }

        Ptr<LispObject> toString(StackPtr<LispObject> pFile)
        {
            typeCheck(pFile, Type::File);

            auto output = ezStringBuilder();

            auto& wrappedFile = pFile->getData<Data>().m_file;
            auto pFileName = pFile->getData<Data>().m_pFileName;

            if (wrappedFile.IsOpen())
            {
                auto& fileNameValue = str::getValue(pFileName);
                output.AppendFormat("<open %s: \"%s\">", getMetaInfo()->getPrettyName(), fileNameValue.GetData());
            }
            else
            {
                if(isNil(pFileName))
                {
                    output.AppendFormat("<closed %s>", getMetaInfo()->getPrettyName());
                }
                else
                {
                    auto& fileNameValue = str::getValue(pFileName);
                    output.AppendFormat("<closed %s: \"%s\">", getMetaInfo()->getPrettyName(), fileNameValue.GetData());
                }
            }

            return str::create(output.GetData());
        }

    }
}
