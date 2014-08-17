#include "stdafx.h"
#include "lcpp/core/builtins/lambda_builtinFunctions.h"
#include "lcpp/core/typeSystem/types/continuation.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/number.h"
#include "lcpp/core/typeSystem/types/bool.h"

#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/stream.h"
#include "lcpp/core/typeSystem/metaInfo.h"
#include "lcpp/core/reader.h"
#include "lcpp/core/evaluator.h"
#include "lcpp/core/runtime.h"
#include "lcpp/core/printer.h"
#include "lcpp/core/exceptions/exitException.h"
#include "lcpp/core/typeSystem/types/file.h"
#include "lcpp/core/typeSystem/types/void.h"
#include "lcpp/core/exceptions/fileException.h"
#include "lcpp/core/exceptions/invalidInputException.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> read(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pToRead = pStack->get(1);

                auto content = ezStringBuilder();

                if (object::isType(pToRead, Type::String))
                {
                    content.Format("(begin %s)", str::getValue(pToRead).GetData());
                }
                else if(object::isType(pToRead, Type::Stream))
                {
                    content.Format("(begin %s)", stream::getIterator(pToRead).GetData());
                }
                else
                {
                    auto message = ezStringBuilder();
                    message.Format("Expected either type \"%s\" or \"%s\", got \"%s\".",
                                   str::metaInfo().getPrettyName(),
                                   stream::metaInfo().getPrettyName(),
                                   object::getMetaInfo(pToRead).getPrettyName());
                    typeCheckFailed(message.GetData());
                }

                auto pContent = str::create(content);
                auto pStream = stream::create(str::getValue(pContent).GetIteratorFront());

                pStack->clear();
                pStack->push(pStream);
                LCPP_cont_tailCall(pCont, &reader::read);
            }

            Ptr<LispObject> eval(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                
                const auto argCount = pStack->size() - 1;

                auto pArg0 = pStack->get(1);
                auto pArg1 = pStack->get(2);

                auto pEnv = pArg0;
                auto pToEval = pArg1;

                pStack->clear();

                if(argCount == 1)
                {
                    auto pState = cont::getRuntimeState(pCont);
                    pEnv = pState->getGlobalEnvironment();
                    pToEval = pArg0;
                }

                pStack->push(pEnv);
                pStack->push(pToEval);

                LCPP_cont_tailCall(pCont, &eval::evaluate);
            }

            Ptr<LispObject> print(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pToPrint = pStack->get(1);
                pStack->clear();
                pStack->push(pToPrint);
                LCPP_cont_tailCall(pCont, &printer::print);
            }

            Ptr<LispObject> exit(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto exitCode = ezInt32(0);

                if (pStack->size() == 2)
                {
                    auto pExitCode = pStack->get(1);
                    exitCode = ezInt32(number::getInteger(pExitCode));
                }

                auto message = ezStringBuilder();
                message.Format("Exiting with exit code %d.", exitCode);
                
                LCPP_THROW(exceptions::Exit(exitCode, message.GetData()));
            }

            Ptr<LispObject> file::open(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);

                auto pFileMode = pStack->get(2);

                if(isNil(pFileMode))
                {
                    pFileMode = str::create("r");
                }

                typeCheck(pFileMode, Type::String);

                // Get the file name.
                //////////////////////////////////////////////////////////////////////////
                auto pFileName = LCPP_pNil;

                if(object::isType(pFile, Type::File))
                {
                    pFileName = lcpp::file::getFileName(pFile);
                }
                else if(object::isType(pFile, Type::String))
                {
                    pFileName = pFile;
                    pFile = lcpp::file::create();
                }
                else
                {
                    ezStringBuilder message;
                    message.AppendFormat("Expected either type \"%s\" or \"%s\", got \"%s\".",
                                         lcpp::file::metaInfo().getPrettyName(),
                                         str::metaInfo().getPrettyName(),
                                         object::getMetaInfo(pFile).getPrettyName());
                    typeCheckFailed(message.GetData());
                }

                // Make sure the file name is absolute.
                //////////////////////////////////////////////////////////////////////////

                ezStringBuilder absoluteFileName;
                absoluteFileName.Append(str::getValue(pFileName).GetData());

                if(absoluteFileName.IsRelativePath())
                {
                    absoluteFileName.MakeAbsolutePath(cont::getRuntimeState(pCont)->getDataDirectory());
                    pFileName = str::create(absoluteFileName);
                }

                // Make sure the file exists.
                //////////////////////////////////////////////////////////////////////////

                if (!ezFileSystem::ExistsFile(absoluteFileName.GetData()))
                {
                    ezStringBuilder message;
                    message.AppendFormat("File does not exist: \"%s\"", absoluteFileName.GetData());
                    LCPP_THROW(exceptions::FileDoesNotExist(message.GetData()));
                }

                auto pResult = lcpp::file::open(pFile, pFileName, pFileMode);

                if (isTrue(pResult))
                {
                    LCPP_cont_return(pCont, pFile);
                }

                lcpp::file::close(pFile);
                LCPP_cont_return(pCont, LCPP_pFalse);
            }

            Ptr<LispObject> file::isOpen(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);
                typeCheck(pFile, Type::File);

                LCPP_cont_return(pCont, lcpp::file::isOpen(pFile));
            }

            Ptr<LispObject> file::close(Ptr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);
                typeCheck(pFile, Type::File);

                lcpp::file::close(pFile);

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

        }
    }
}
