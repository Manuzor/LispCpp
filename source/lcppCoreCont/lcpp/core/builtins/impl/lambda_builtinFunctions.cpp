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
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    namespace lambda
    {
        namespace builtin
        {
            Ptr<LispObject> read(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                StackPtr<LispObject> pToRead = cont::getStack(pCont)->get(1);

                ezStringBuilder content;

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
                    ezStringBuilder message;
                    message.Format("Expected either type \"%s\" or \"%s\", got \"%s\".",
                                   str::getMetaInfo()->getPrettyName(),
                                   stream::getMetaInfo()->getPrettyName(),
                                   object::getMetaInfo(pToRead).getPrettyName());
                    typeCheckFailed(message.GetData());
                }

                StackPtr<LispObject> pContent = str::create(content.GetData(), content.GetElementCount());
                StackPtr<LispObject> pStream = stream::create(pContent);

                cont::getStack(pCont)->clear();
                cont::getStack(pCont)->push(pStream);
                LCPP_cont_tailCall(pCont, &reader::read);
            }

            Ptr<LispObject> eval(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> print(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pToPrint = pStack->get(1);

                cont::setFunction(pCont, &lcpp::printer::lineBreak);
                LCPP_cont_call(pCont, &lcpp::printer::print, pToPrint);
            }

            Ptr<LispObject> exit(StackPtr<LispObject> pCont)
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

            Ptr<LispObject> cons(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pCar = pStack->get(1);
                auto pCdr = pStack->get(2);

                auto pCons = lcpp::cons::create(pCar, pCdr);

                LCPP_cont_return(pCont, pCons);
            }

            Ptr<LispObject> car(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pCons = pStack->get(1);
                typeCheck(pCons, Type::Cons);

                auto pCar = lcpp::cons::getCar(pCons);

                LCPP_cont_return(pCont, pCar);
            }

            Ptr<LispObject> cdr(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pCons = pStack->get(1);
                typeCheck(pCons, Type::Cons);

                auto pCdr = lcpp::cons::getCdr(pCons);

                LCPP_cont_return(pCont, pCdr);
            }

            Ptr<LispObject> list(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);

                auto pCons = lcpp::cons::pack(pCont, 1);

                LCPP_cont_return(pCont, pCons);
            }

            Ptr<LispObject> eqq(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pArg0 = pStack->get(1);
                auto pArg1 = pStack->get(2);

                auto pResult = pArg0 == pArg1 ? LCPP_pTrue : LCPP_pFalse;

                LCPP_cont_return(pCont, pResult);
            }

            Ptr<LispObject> recursionLimit(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);
                auto pState = cont::getRuntimeState(pCont);

                auto argCount = pStack->size() - 1;

                if (argCount == 0)
                {
                    auto recursionLimit = pState->getRecursionLimit();
                    LCPP_cont_return(pCont, number::create(recursionLimit));
                }

                auto pRecursionLimit = pStack->get(1);

                // In case the recursion limit integer type changes
                typedef decltype(pState->getRecursionLimit()) RecursionLimit_t;
                EZ_CHECK_AT_COMPILETIME(std::is_integral<RecursionLimit_t>::value);

                auto recursionLimit = RecursionLimit_t(number::getInteger(pRecursionLimit));
                pState->setRecursionLimit(recursionLimit);
                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            Ptr<LispObject> file::open(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                StackPtr<LispObject> pFile = pStack->get(1);

                StackPtr<LispObject> pFileMode = pStack->get(2);

                if(isNil(pFileMode))
                {
                    pFileMode = str::create("r");
                }

                typeCheck(pFileMode, Type::String);

                // Get the file name.
                //////////////////////////////////////////////////////////////////////////
                StackPtr<LispObject> pFileName = LCPP_pNil;

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
                                         lcpp::file::getMetaInfo()->getPrettyName(),
                                         str::getMetaInfo()->getPrettyName(),
                                         object::getMetaInfo(pFile).getPrettyName());
                    typeCheckFailed(message.GetData());
                }

                // Make sure the file name is absolute.
                //////////////////////////////////////////////////////////////////////////

                ezStringBuilder absoluteFileName;
                absoluteFileName.Append(str::getValue(pFileName).GetData());

                if(absoluteFileName.IsRelativePath())
                {
                    absoluteFileName.MakeAbsolutePath(cont::getRuntimeState(pCont)->getUserDirectory());
                    pFileName = str::create(absoluteFileName.GetData(), absoluteFileName.GetElementCount());
                }

                // Make sure the file exists.
                //////////////////////////////////////////////////////////////////////////

                if (!ezFileSystem::ExistsFile(absoluteFileName.GetData()))
                {
                    ezStringBuilder message;
                    message.AppendFormat("File does not exist: \"%s\"", absoluteFileName.GetData());
                    LCPP_THROW(exceptions::FileDoesNotExist(message.GetData()));
                }

                StackPtr<LispObject> pResult = lcpp::file::open(pFile, pFileName, pFileMode);

                if (isTrue(pResult))
                {
                    LCPP_cont_return(pCont, pFile);
                }

                lcpp::file::close(pFile);
                LCPP_cont_return(pCont, LCPP_pFalse);
            }

            Ptr<LispObject> file::isOpen(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);
                typeCheck(pFile, Type::File);

                LCPP_cont_return(pCont, lcpp::file::isOpen(pFile));
            }

            Ptr<LispObject> file::close(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);
                typeCheck(pFile, Type::File);

                lcpp::file::close(pFile);

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

            Ptr<LispObject> file::readString(StackPtr<LispObject> pCont)
            {
                typeCheck(pCont, Type::Continuation);
                auto pState = cont::getRuntimeState(pCont);
                auto pStack = cont::getStack(pCont);

                StackPtr<LispObject> pFileName = pStack->get(1);
                typeCheck(pFileName, Type::String);

                auto szFileNameValue = str::getValue(pFileName).GetData();

                auto pString = LCPP_pNil;

                {
                    ezStringBuilder absoluteFileName(szFileNameValue);
                    absoluteFileName.MakeAbsolutePath(pState->getUserDirectory());

                    ezFileReader fileReader;
                    auto openingTheFile = fileReader.Open(absoluteFileName.GetData());

                    if(openingTheFile.Failed())
                    {
                        ezStringBuilder message;
                        message.AppendFormat("Failed to open file \"%s\".", szFileNameValue);
                        LCPP_THROW(exceptions::UnableToOpenFile(message.GetData()));
                    }

                    auto fileSize64 = fileReader.GetFileSize();
                    auto fileSize32 = ezUInt32(fileSize64);

                    ezHybridArray<ezUInt8, 256> rawFileContent;
                    rawFileContent.SetCount(fileSize32 + 1);
                    fileReader.ReadBytes(&rawFileContent[0], fileSize32);

                    rawFileContent[fileSize32] = '\0';

                    auto szString = reinterpret_cast<const char*>(&rawFileContent[0]);
                    pString = str::create(szString, fileSize32);
                }

                LCPP_cont_return(pCont, pString);
            }

            Ptr<LispObject> file::eval(StackPtr<LispObject> pCont)
            {
                LCPP_NOT_IMPLEMENTED;

                typeCheck(pCont, Type::Continuation);
                auto pStack = cont::getStack(pCont);

                auto pFile = pStack->get(1);

                if (object::isType(pFile, Type::String))
                {
                }


                typeCheck(pFile, Type::File);

                lcpp::file::close(pFile);

                LCPP_cont_return(pCont, LCPP_pVoid);
            }

        }
    }
}
